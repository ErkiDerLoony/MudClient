/*
 * © Copyright 2008 by Edgar Kalkowski (eMail@edgar-kalkowski.de)
 * 
 * This file is part of the JMudClient.
 * 
 * The JMudClient is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 3 of the License, or (at your option) any
 * later version.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

package erki.mudclient;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Font;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.MouseWheelEvent;
import java.awt.event.MouseWheelListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PrintStream;
import java.io.PrintWriter;
import java.util.LinkedList;

import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextField;
import javax.swing.JTextPane;
import javax.swing.ScrollPaneConstants;
import javax.swing.Timer;
import javax.swing.UIManager;
import javax.swing.UnsupportedLookAndFeelException;
import javax.swing.border.EmptyBorder;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import javax.swing.text.AttributeSet;
import javax.swing.text.BadLocationException;

import erki.api.util.Localizor;
import erki.api.util.Log;

/**
 * This class implements a swing gui for the client.
 * 
 * @author Edgar Kalkowski
 */
public class SwingUi extends JFrame {
    
    /** The default background colour used for the output {@link JTextPane}. */
    public static final Color DEFAULT_BACKGROUND = Color.BLACK;
    
    /** The default foregound colour used for the output {@link JTextPane}. */
    public static final Color DEFAULT_FOREGROUND = new Color(127, 200, 255);
    
    private static final File SETTINGS_FILE = new File(System
            .getProperty("user.home")
            + System.getProperty("file.separator") + ".jmudguirc")
            .getAbsoluteFile();
    
    /** {@link Font} used for the input line at the bottom of the screen. */
    public static final Font INPUT_FONT = new Font(Font.MONOSPACED, Font.PLAIN,
            14);
    
    /** {@link Font} used for the output box. */
    public static final Font OUTPUT_FONT = INPUT_FONT;
    
    /** In tribute to the api. **/
    private static final long serialVersionUID = -6478785594016594970L;
    
    private LinkedList<String> history = new LinkedList<String>();
    
    private int index = 0;
    
    private JTextPane output;
    
    private JScrollPane scrollPane;
    
    private JTextField input;
    
    private boolean scroll = true;
    
    private Timer timer;
    
    /** Create a new {@code SwingUI}. Loads gui settings and shows the gui. */
    public SwingUi() {
        history.add("");
        
        // Try to change the look and feel to gtk as it looks better.
        try {
            Log.print(this, "Trying to switch to gtk look and feel.");
            UIManager.setLookAndFeel("com.sun.java.swing.plaf.gtk."
                    + "GTKLookAndFeel");
        } catch (ClassNotFoundException e) {
            Log.error(this, e);
            Log.info(this, "GTK look and feel could not be found on your "
                    + "system.");
        } catch (InstantiationException e) {
            Log.error(this, e);
            Log.info(this, "GTK look and feel could not be instanciated.");
        } catch (IllegalAccessException e) {
            Log.error(this, e);
            Log.info(this, "You are not allowed to change the look and feel.");
        } catch (UnsupportedLookAndFeelException e) {
            Log.error(this, e);
            Log.info(this, "GTK look and feel is not supported by you system.");
        }
        
        setTitle("JMudClient v" + Controller.VERSION);
        Log.print(this, "Creating gui.");
        
        // The order of this statements is crucial as createListeners() works
        // with the components created before.
        createComponents();
        createListeners();
        
        pack();
        
        // Load settings after creating the gui.
        loadSettings();
        
        // Make sure the user stays online.
        timer = new Timer(300000, new ActionListener() {
            
            @Override
            public void actionPerformed(ActionEvent e) {
                Controller.getInstance().send(getPhrase());
            }
        });
        
        timer.start();
        
        // Show the main window.
        setVisible(true);
        input.requestFocusInWindow();
    }
    
    private void createComponents() {
        setLayout(new BorderLayout());
        
        // Add output panel.
        output = new JTextPane();
        output.setBackground(DEFAULT_BACKGROUND);
        output.setForeground(DEFAULT_FOREGROUND);
        output.setFont(SwingUi.OUTPUT_FONT);
        output.setEditable(false);
        
        scrollPane = new JScrollPane(output);
        scrollPane
                .setHorizontalScrollBarPolicy(ScrollPaneConstants.HORIZONTAL_SCROLLBAR_NEVER);
        
        JPanel wrapperPanel = new JPanel();
        wrapperPanel.setLayout(new BorderLayout());
        wrapperPanel.setBorder(new EmptyBorder(10, 10, 10, 10));
        wrapperPanel.add(scrollPane, BorderLayout.CENTER);
        add(wrapperPanel, BorderLayout.CENTER);
        
        // Add input line.
        input = new JTextField();
        input.setFont(SwingUi.INPUT_FONT);
        input.requestFocus();
        
        wrapperPanel = new JPanel();
        wrapperPanel.setLayout(new BorderLayout());
        wrapperPanel.setBorder(new EmptyBorder(0, 10, 10, 10));
        wrapperPanel.add(input, BorderLayout.CENTER);
        add(wrapperPanel, BorderLayout.SOUTH);
    }
    
    private void createListeners() {
        
        addWindowListener(new WindowAdapter() {
            
            @Override
            public void windowClosing(WindowEvent e) {
                super.windowClosing(e);
                Controller.getInstance().quit();
            }
            
            @Override
            public void windowClosed(WindowEvent e) {
                super.windowClosed(e);
                saveSettings();
            }
        });
        
        input.addKeyListener(new KeyAdapter() {
            
            private int keycode;
            
            @Override
            public void keyPressed(KeyEvent e) {
                super.keyPressed(e);
                keycode = e.getKeyCode();
                timer.restart();
                
                Log.debug(SwingUi.class, history.toString());
                
                if (!e.isActionKey() && index != history.size() - 1) {
                    
                    if (history.getLast().equals("")) {
                        history.set(history.size() - 1, input.getText());
                    } else {
                        history.addLast(input.getText());
                    }
                    
                    index = history.size() - 1;
                }
                
                if (keycode == KeyEvent.VK_UP && index > 0) {
                    history.set(index, input.getText());
                    index--;
                    input.setText(history.get(index));
                }
                
                if (keycode == KeyEvent.VK_DOWN && index < history.size() - 1) {
                    history.set(index, input.getText());
                    index++;
                    input.setText(history.get(index));
                }
            }
            
            @Override
            public void keyTyped(KeyEvent e) {
                super.keyTyped(e);
                
                if (keycode == KeyEvent.VK_ENTER) {
                    Controller.getInstance().println(input.getText(),
                            Formatter.getOwnInputFormat());
                    history.set(index, input.getText());
                    parse(input.getText());
                    input.setText("");
                    history.addLast("");
                    index = history.size() - 1;
                }
                
                if ((keycode == KeyEvent.VK_Q) && e.isControlDown()) {
                    Controller.getInstance().quit();
                }
            }
        });
        
        scrollPane.getVerticalScrollBar().getModel().addChangeListener(
                new ChangeListener() {
                    
                    @Override
                    public void stateChanged(ChangeEvent e) {
                        
                        if (scroll) {
                            scrollPane.getVerticalScrollBar().setValue(
                                    scrollPane.getVerticalScrollBar()
                                            .getMaximum());
                        }
                    }
                });
        
        scrollPane.getVerticalScrollBar().addMouseListener(new MouseAdapter() {
            
            @Override
            public void mousePressed(MouseEvent e) {
                super.mousePressed(e);
                scroll = false;
            }
        });
        
        scrollPane.addMouseWheelListener(new MouseWheelListener() {
            
            @Override
            public void mouseWheelMoved(MouseWheelEvent e) {
                scroll = false;
            }
        });
    }
    
    private void parse(String input) {
        
        if (input.toLowerCase().equals("/quit")) {
            Controller.getInstance().quit();
            return;
        }
        
        if (input.toLowerCase().startsWith("/connect ")) {
            int port = 23;
            String host = input.substring("/connect ".length());
            
            if (host.contains(" ")) {
                
                try {
                    port = Integer.parseInt(host.substring(
                            host.indexOf(' ') + 1).trim());
                    host = host.substring(0, host.indexOf(' ')).trim();
                } catch (NumberFormatException e) {
                    port = 23;
                }
            }
            
            if (host.contains(":")) {
                
                try {
                    port = Integer.parseInt(host.substring(
                            host.indexOf(':') + 1).trim());
                    host = host.substring(host.indexOf(':')).trim();
                } catch (NumberFormatException e) {
                    port = 23;
                }
            }
            
            Controller.getInstance().connect(host, port);
            return;
        }
        
        if (input.toLowerCase().equals("/connect")) {
            Controller.getInstance().println(
                    Localizor.getInstance().get("HOST_NEEDED"),
                    Formatter.getErrorFormat());
            return;
        }
        
        if (input.toLowerCase().equals("/disconnect")) {
            Controller.getInstance().disconnect();
            return;
        }
        
        if (input.toLowerCase().equals("/center")) {
            setLocationRelativeTo(null);
            return;
        }
        
        Controller.getInstance().send(input);
    }
    
    private String getPhrase() {
        LinkedList<String> smileList = new LinkedList<String>();
        smileList.add("cutely");
        smileList.add("manically");
        smileList.add("patiently");
        smileList.add("innocently");
        smileList.add("kindly");
        smileList.add("politely");
        smileList.add("brightly");
        
        LinkedList<String> blinkList = new LinkedList<String>();
        blinkList.add("full of boredom");
        blinkList.add("amazed of the silence");
        
        LinkedList<String> mutterList = new LinkedList<String>();
        mutterList.add("youth");
        mutterList.add("dark robes");
        mutterList.add("little green elves");
        mutterList.add("dark spydry thyngs");
        mutterList.add("a stolen crucifix");
        mutterList.add("HIM");
        mutterList.add("control and alternate delete");
        
        LinkedList<String> scratchList = new LinkedList<String>();
        scratchList.add("nose");
        scratchList.add("face");
        scratchList.add("finger");
        scratchList.add("cheek");
        scratchList.add("ears");
        
        LinkedList<String> crackList = new LinkedList<String>();
        crackList.add("nose");
        crackList.add("neck");
        crackList.add("backbones");
        crackList.add("fingers");
        crackList.add("knees");
        
        switch ((int) (Math.random() * 5)) {
            case 0:
                return "smile " + smileList.get((int) (Math.random() * smileList.size()));
            case 1:
                return "blink " + blinkList.get((int) (Math.random() * blinkList.size()));
            case 2:
                return "mutter " + mutterList.get((int) (Math.random() * mutterList.size()));
            case 3:
                return "scratch " + scratchList.get((int) (Math.random() * scratchList.size()));
            case 4:
                return "crack " + crackList.get((int) (Math.random() * crackList.size()));
            default:
                return "look grim";
        }
    }
    
    public void appendToOutput(String text, AttributeSet format) {
        // Scroll down automatically only if user did not scroll
        // up. We do not want to disturb the user reading old
        // lines.
        scrollPane.revalidate();
        scroll = (scrollPane.getVerticalScrollBar().getValue()
                + scrollPane.getVerticalScrollBar().getModel().getExtent() == scrollPane
                .getVerticalScrollBar().getMaximum());
        
        try {
            output.getDocument().insertString(output.getDocument().getLength(),
                    text, format);
        } catch (BadLocationException e) {
            // This cannot happen since the text is always inserted at the end
            // of the document.
        }
    }
    
    private void loadSettings() {
        
        try {
            BufferedReader fileIn = new BufferedReader(new InputStreamReader(
                    new FileInputStream(SETTINGS_FILE)));
            String line;
            
            while ((line = fileIn.readLine()) != null) {
                
                if (line.startsWith("posX = ")) {
                    setLocation(Integer.parseInt(line.substring("posX = "
                            .length())), getY());
                }
                
                if (line.startsWith("posY = ")) {
                    setLocation(getX(), Integer.parseInt(line
                            .substring("posY = ".length())));
                }
                
                if (line.startsWith("width = ")) {
                    setSize(Integer.parseInt(line
                            .substring("width = ".length())), getHeight());
                }
                
                if (line.startsWith("height = ")) {
                    setSize(getWidth(), Integer.parseInt(line
                            .substring("height = ".length())));
                }
            }
            
            fileIn.close();
            Log.print(this, "Loaded gui settings from " + SETTINGS_FILE + ".");
        } catch (FileNotFoundException e) {
            Log.print(this, "No gui settings file found at " + SETTINGS_FILE
                    + ". Falling back to default settings.");
        } catch (IOException e) {
            Log.warning(this, "Could not open " + SETTINGS_FILE
                    + " to load the gui settings!");
            Log.print(this, "Falling back to default settings.");
        }
    }
    
    private void saveSettings() {
        
        try {
            PrintWriter fileOut = new PrintWriter(new OutputStreamWriter(
                    new PrintStream(SETTINGS_FILE)));
            fileOut.println("GUI settings file for the JMudClient.");
            fileOut.println("Do not edit it unless you know what you are "
                    + "doing!");
            fileOut.println("posX = " + getX());
            fileOut.println("posY = " + getY());
            fileOut.println("width = " + getWidth());
            fileOut.println("height = " + getHeight());
            fileOut.close();
            Log.print(this, "Saving gui settings to " + SETTINGS_FILE + ".");
        } catch (FileNotFoundException e) {
            Log.warning(this, "Could not open " + SETTINGS_FILE
                    + " to save the gui settings!");
        }
    }
}
