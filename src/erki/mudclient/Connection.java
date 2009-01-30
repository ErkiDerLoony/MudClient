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

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.Socket;
import java.net.UnknownHostException;

import javax.swing.Timer;

import erki.api.util.Localizor;
import erki.api.util.Log;

/**
 * This class represents a connection to a server.
 * 
 * @author Edgar Kalkowski
 */
public class Connection extends Thread {
    
    private PrintWriter socketOut = null;
    
    private final ServerInputParser parser = new ServerInputParser();
    
    private final String host;
    
    private final int port;
    
    private Timer timer;
    
    private Socket socket;
    
    /**
     * Create a new {@code Connection} to a specific server.
     * 
     * @param host
     *        The host to connect to.
     * @param port
     *        The port to connect to.
     */
    public Connection(String host, int port) {
        this.host = host;
        this.port = port;
    }
    
    @Override
    public void run() {
        Controller.getInstance().println(
                Localizor.getInstance().get("TRY_CONNECT", host, "" + port),
                Formatter.getInfoFormat());
        Log.print(this, "Trying to connect to “" + host + "” on port " + port
                + ".");
        
        // Try to make connection.
        try {
            socket = new Socket(host, port);
            final BufferedReader socketIn = new BufferedReader(
                    new InputStreamReader(socket.getInputStream(), "UTF-8"));
            socketOut = new PrintWriter(new OutputStreamWriter(socket
                    .getOutputStream(), "UTF-8"), true);
            
            Log.print(this, "Connection established.");
            Controller.getInstance().println(
                    Localizor.getInstance().get("CONNECTED"),
                    Formatter.getInfoFormat());
            int b;
            
            try {
                
                while ((b = socketIn.read()) != -1) {
                    String buffer = new String(Character.toChars(b));
                    
                    while (socketIn.ready()) {
                        buffer += new String(Character.toChars(socketIn.read()));
                    }
                    
                    Log.debug(this, "Received “" + buffer
                            + "” from the server.");
                    parser.parse(buffer.toCharArray());
                }
                
            } catch (IOException e) {
                Controller.getInstance().println(
                        Localizor.getInstance().get("IO_ERROR_WHILE_READING",
                                e.getLocalizedMessage()),
                        Formatter.getErrorFormat());
                Log.error(this, e);
            }
            
        } catch (UnknownHostException e) {
            Controller.getInstance().println(
                    Localizor.getInstance().get("LOOKUP_ERROR", host),
                    Formatter.getErrorFormat());
            Log.error(this, e);
        } catch (IOException e) {
            Controller.getInstance().println(
                    Localizor.getInstance().get("IO_ERROR_WHILE_CONNECTING",
                            host, e.getLocalizedMessage()),
                    Formatter.getErrorFormat());
            Log.error(this, e);
        }
        
        Controller.getInstance().disconnected();
        Controller.getInstance().println(
                Localizor.getInstance().get("CONNECTION_LOST", host),
                Formatter.getInfoFormat());
        Log.print(this, "Connection to server lost!");
        
        if (timer != null) {
            timer.stop();
        }
    }
    
    /**
     * Send some text to the server.
     * 
     * @param text
     *        The text to send.
     * @return {@code true} if a connection exists and the text was sent
     *         successfully and {@code false} otherwise.
     */
    public boolean send(String text) {
        
        if (socketOut != null) {
            Log.debug(this, "Sending “" + text + "” to the server.");
            socketOut.println(text);
            return true;
        } else {
            Log.debug(this, "Could not send “" + text + "” to the server.");
            return false;
        }
    }
    
    /**
     * Send the quit signal to the server and thus let the server close the
     * connection (remember that in a client/server architecture the client
     * never terminates a connection to the server).
     */
    public void close() {
        Log.print(this, "Closing connection to “" + host + "“.");
        Controller.getInstance().println(
                Localizor.getInstance().get("CLOSING_CONNECTION", host),
                Formatter.getInfoFormat());
        
        if (!send("QUIT")) {
            Log.debug(Connection.this, "Trying to interrupt connection.");
            
            if (socket != null) {
                
                try {
                    socket.close();
                } catch (IOException e) {
                }
            }
        }
        
        timer = new Timer(10000, new ActionListener() {
            
            @Override
            public void actionPerformed(ActionEvent e) {
                Log.debug(Connection.this, "Trying to interrupt connection.");
                
                if (socket != null) {
                    
                    try {
                        socket.close();
                    } catch (IOException e1) {
                    }
                }
            }
        });
        
        timer.setRepeats(false);
        timer.start();
    }
}
