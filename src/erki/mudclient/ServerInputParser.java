/*
 * © Copyright 2009–2010, 2012 by Edgar Kalkowski (eMail@edgar-kalkowski.de)
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

import java.awt.Color;

import javax.swing.text.SimpleAttributeSet;
import javax.swing.text.StyleConstants;

import erki.api.util.Log;

/**
 * Used by {@link Connection} to parse the input received from the server.
 * 
 * @author Edgar Kalkowski
 */
public class ServerInputParser {
    
    private enum State {
        SCANNING, WAIT_FOR_BRACKET, WAIT_FOR_FIRST_NUMBER, ACCUMULATE_NUMBERS
    }
    
    private StringBuffer buffer = new StringBuffer();
    
    private State state = State.SCANNING;
    
    private SimpleAttributeSet format = new SimpleAttributeSet();
    
    /**
     * Parse a byte of input. The byte is interpreted by
     * {@link Character#toChars(int)}.
     * 
     * @param b
     *        The byte of input to parse.
     */
    public void parse(char[] chars) {
        
        for (char c : chars) {
            
            switch (state) {
                case SCANNING:

                    if (c == '\033') {
                        state = State.WAIT_FOR_BRACKET;
                    } else {
                        Controller.getInstance().print("" + c, format);
                    }
                    
                    break;
                case WAIT_FOR_BRACKET:

                    if (c == '[') {
                        state = State.WAIT_FOR_FIRST_NUMBER;
                    } else {
                        state = State.SCANNING;
                    }
                    
                    break;
                case WAIT_FOR_FIRST_NUMBER:

                    if (Character.isDigit(c)) {
                        buffer.append(c);
                        state = State.ACCUMULATE_NUMBERS;
                    } else if (c == 'm') {
                        format = new SimpleAttributeSet();
                        state = State.SCANNING;
                    }
                    
                    break;
                case ACCUMULATE_NUMBERS:
                    
                    if (c == ';') {
                        evaluate(buffer.toString());
                        buffer = new StringBuffer();
                    } else if (c == 'm') {
                        evaluate(buffer.toString());
                        buffer = new StringBuffer();
                        state = State.SCANNING;
                    } else {
                        buffer.append(c);
                    }
                    
                    break;
            }
        }
    }
    
    private void evaluate(String number) {
        
        try {
            int no = Integer.parseInt(number);
            
            switch (no) {
                case 0:
                    format = new SimpleAttributeSet();
                    break;
                case 1:
                    StyleConstants.setBold(format, true);
                    break;
                case 2: // Ignore faint intensity (not supported).
                    break;
                case 3:
                    StyleConstants.setItalic(format, true);
                    break;
                case 4:
                    StyleConstants.setUnderline(format, true);
                    break;
                case 5: // Ignore slow blink (not supported).
                    break;
                case 6: // Ignore fast blink (not supported).
                    break;
                case 7:
                    Color fg = StyleConstants.getForeground(format);
                    Color bg = StyleConstants.getBackground(format);
                    StyleConstants.setForeground(format, bg);
                    StyleConstants.setBackground(format, fg);
                    break;
                case 8: // Ignore conceal (not supported).
                    break;
                case 21: // Ignore double underline (not supported).
                    break;
                case 22:
                    StyleConstants.setBold(format, false);
                    break;
                case 24:
                    StyleConstants.setUnderline(format, false);
                    break;
                case 25: // Ignore disable blink (not supported).
                    break;
                case 27:
                    fg = StyleConstants.getForeground(format);
                    bg = StyleConstants.getBackground(format);
                    StyleConstants.setForeground(format, bg);
                    StyleConstants.setBackground(format, fg);
                    break;
                case 28: // Ignore conceal off (not supported
                    break;
                case 30:
                    StyleConstants.setForeground(format, Color.BLACK);
                    break;
                case 31:
                    StyleConstants.setForeground(format, Color.RED.darker()
                            .darker());
                    break;
                case 32:
                    StyleConstants.setForeground(format, Color.GREEN.darker()
                            .darker());
                    break;
                case 33:
                    StyleConstants.setForeground(format, Color.YELLOW.darker()
                            .darker());
                    break;
                case 34:
                    StyleConstants.setForeground(format, Color.BLUE.darker()
                            .darker());
                    break;
                case 35:
                    StyleConstants.setForeground(format, Color.MAGENTA.darker()
                            .darker());
                    break;
                case 36:
                    StyleConstants.setForeground(format, Color.CYAN.darker()
                            .darker());
                    break;
                case 37:
                    StyleConstants.setForeground(format, Color.WHITE.darker()
                            .darker());
                    break;
                case 39:
                    StyleConstants.setForeground(format,
                            SwingUi.DEFAULT_FOREGROUND);
                    break;
                case 40:
                    StyleConstants.setBackground(format, Color.BLACK);
                    break;
                case 41:
                    StyleConstants.setBackground(format, Color.RED.darker()
                            .darker());
                    break;
                case 42:
                    StyleConstants.setBackground(format, Color.GREEN.darker()
                            .darker());
                    break;
                case 43:
                    StyleConstants.setBackground(format, Color.YELLOW.darker()
                            .darker());
                    break;
                case 44:
                    StyleConstants.setBackground(format, Color.BLUE.darker()
                            .darker());
                    break;
                case 45:
                    StyleConstants.setBackground(format, Color.MAGENTA.darker()
                            .darker());
                    break;
                case 46:
                    StyleConstants.setBackground(format, Color.CYAN.darker()
                            .darker());
                    break;
                case 47:
                    StyleConstants.setBackground(format, Color.WHITE.darker()
                            .darker());
                    break;
                case 49:
                    StyleConstants.setBackground(format,
                            SwingUi.DEFAULT_BACKGROUND);
                    break;
                case 90:
                    StyleConstants.setForeground(format, Color.GRAY);
                    break;
                case 91:
                    StyleConstants.setForeground(format, Color.RED);
                    break;
                case 92:
                    StyleConstants.setForeground(format, Color.GREEN);
                    break;
                case 93:
                    StyleConstants.setForeground(format, Color.YELLOW);
                    break;
                case 94:
                    StyleConstants.setForeground(format, Color.BLUE);
                    break;
                case 95:
                    StyleConstants.setForeground(format, Color.MAGENTA);
                    break;
                case 96:
                    StyleConstants.setForeground(format, Color.CYAN);
                    break;
                case 97:
                    StyleConstants.setForeground(format, Color.WHITE);
                    break;
                case 99:
                    StyleConstants.setForeground(format,
                            SwingUi.DEFAULT_FOREGROUND);
                    break;
                case 100:
                    StyleConstants.setBackground(format, Color.GRAY);
                    break;
                case 101:
                    StyleConstants.setBackground(format, Color.RED);
                    break;
                case 102:
                    StyleConstants.setBackground(format, Color.GREEN);
                    break;
                case 103:
                    StyleConstants.setBackground(format, Color.YELLOW);
                    break;
                case 104:
                    StyleConstants.setBackground(format, Color.BLUE);
                    break;
                case 105:
                    StyleConstants.setBackground(format, Color.MAGENTA);
                    break;
                case 106:
                    StyleConstants.setBackground(format, Color.CYAN);
                    break;
                case 107:
                    StyleConstants.setBackground(format, Color.WHITE);
                    break;
                case 109:
                    StyleConstants.setBackground(format,
                            SwingUi.DEFAULT_BACKGROUND);
                    break;
                default:
                    Log.warning(this, "I don't know the control code “"
                            + number + "”!");
            }
            
        } catch (NumberFormatException e) {
            Log.warning(this, "Could not parse control code “" + number + "”!");
        }
    }
}
