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

import java.util.TreeMap;

import javax.swing.SwingUtilities;
import javax.swing.text.AttributeSet;

import erki.api.util.CommandLineParser;
import erki.api.util.Localizor;
import erki.api.util.Log;

/**
 * This class is the controller that intermediates between gui and connection
 * and knows both.
 * 
 * @author Edgar Kalkowski
 */
public class Controller {
    
    /** The version of this program. For use where needed. */
    public static final String VERSION = "0.5.0";
    
    /** This is the one and only instance of the controller. */
    private static Controller instance;
    
    /** The connection to a MUD server if one is established. */
    private Connection connection = null;
    
    /**
     * If {@code true} indicates that everything was just waiting for closing
     * connections to quit.
     */
    private boolean wasQuit = false;
    
    /** The gui that displays text. */
    private SwingUi gui;
    
    /** Set to true on program exit; makes additional threads exit. */
    private boolean killed = false;

    private Thread healthThread;
    
    /**
     * Adhering the singleton design pattern this constructor is private and
     * only called once from the main method. Initializes a user interface.
     * Other classes can obtain an instance of this class later by calling the
     * {@link #getInstance()} method.
     */
    private Controller() {
        Controller.instance = this;
        
        // Start user interface.
        SwingUtilities.invokeLater(new Runnable() {
            
            @Override
            public void run() {
                gui = new SwingUi();
            }
        });
        
        // Start a new thread that (if connected) occasionally requests the
        // user's health points etc.
        healthThread = new Thread("HealthThread") {
            
            @Override
            public void run() {
                super.run();
                
                while (!killed) {
                    
                    if (connection != null) {
                        connection.send("score verbose");
                    }
                    
                    try {
                        Thread.sleep(300000);
                    } catch (InterruptedException e) {
                    }
                }
            }
            
        };
        
        // TODO: Start health thread if parsing of health line is implemented.
        //healthThread.start();
    }
    
    /**
     * @return the one and only instance of this class (adhering to the
     *         singleton design pattern).
     */
    public static Controller getInstance() {
        return instance;
    }
    
    /**
     * Checks that all connections are cleanly terminated before really exiting
     * the program.
     */
    public void quit() {
        
        if (connection != null) {
            wasQuit = true;
            connection.close();
        } else {
            gui.dispose();
            killed = true;
            healthThread.interrupt();
        }
    }
    
    /**
     * Opens a new connection to a MUD server.
     * 
     * @param host
     *        The canonical hostname of the MUD server.
     * @param port
     *        The port to connect to.
     */
    public void connect(String host, int port) {
        
        if (connection != null) {
            connection.close();
        }
        
        connection = new Connection(host, port);
        connection.start();
    }
    
    /**
     * Send some text to the server. If the sending does not succeed or no
     * connection has been established so far an error message is output via the
     * gui.
     * 
     * @param text
     *        The text to send to the server.
     */
    public void send(String text) {
        
        if (connection == null) {
            println(Localizor.getInstance().get("NOT_YET_CONNECTED"), Formatter
                    .getErrorFormat());
            Log.error(this, "Could not send to server because no connection "
                    + "has been established so far!");
        } else {
            
            if (!connection.send(text)) {
                println(Localizor.getInstance().get("COULD_NOT_SEND"),
                        Formatter.getErrorFormat());
                Log.error(this, "Could not send to server!");
            }
        }
    }
    
    /**
     * Disconnect from the server if a connection exists. If no connection
     * exists this method does nothing.
     */
    public void disconnect() {
        
        if (connection != null) {
            connection.close();
        }
    }
    
    /** Called by {@link Connection} if the connection to the server is lost. */
    public void disconnected() {
        connection = null;
        
        if (wasQuit) {
            gui.dispose();
            killed = true;
            healthThread.interrupt();
        }
    }
    
    /**
     * Append some text to the gui output. All methods that output something via
     * the gui are synchronized to prevent server input from being mixed up with
     * user input as the server input comes from a different thread.
     * 
     * @param text
     *        The text to print.
     */
    public synchronized void print(String text) {
        gui.appendToOutput(text, Formatter.getDefaultFormat());
    }
    
    /**
     * Append some text with a specific format to the gui output.
     * 
     * @see #print(String)
     * @param text
     *        The text to print.
     * @param format
     *        An {@link AttributeSet} describing the format of the text.
     */
    public synchronized void print(String text, AttributeSet format) {
        gui.appendToOutput(text, format);
    }
    
    /**
     * Append a line of text to the gui output.
     * 
     * @see #print(String)
     * @param line
     *        The line of text to print.
     */
    public synchronized void println(String line) {
        gui.appendToOutput(line + "\n", Formatter.getDefaultFormat());
    }
    
    /**
     * Append a line of text with a specific format to the gui output.
     * 
     * @see #print(String)
     * @param line
     *        The line of text to print.
     * @param format
     *        An {@link AttributeSet} describing the format of the text.
     */
    public synchronized void println(String line, AttributeSet format) {
        gui.appendToOutput(line + "\n", format);
    }
    
    /**
     * Start the JMudClient.
     * 
     * @param arguments
     *        The command line arguments. Which arguments the program will
     *        accept is displayed when starting it with »--help«.
     */
    public static void main(String[] arguments) {
        TreeMap<String, String> args = CommandLineParser.parse(arguments);
        
        if (args.containsKey("--help")) {
            printHelp();
            return;
        }
        
        if (args.containsKey("--debug")) {
            Log.setDebug(true);
            Log.debug(Controller.class, "Enabling debug output.");
            args.remove("--debug");
        }
        
        if (!args.keySet().isEmpty()) {
            
            for (String key : args.keySet()) {
                Log.warning(Controller.class, "Unknown command line "
                        + "argument: “" + key + " " + args.get(key) + "”.");
            }
        }
        
        // This must be the one and only place the controller ist instanciated!
        new Controller();
    }
    
    private static void printHelp() {
        System.out.println("This is JMudClient v" + VERSION);
        System.out.println("Usage: java Controller [--debug] [--help]");
    }
}
