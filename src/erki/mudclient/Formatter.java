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

import java.awt.Color;

import javax.swing.text.AttributeSet;
import javax.swing.text.SimpleAttributeSet;
import javax.swing.text.StyleConstants;

/**
 * This class provides some static utility methods that return
 * {@link AttributeSet}s that can be used to format text in a special way.
 * 
 * @author Edgar Kalkowski
 */
public class Formatter {
    
    /**
     * @return An {@code AttributeSet} that actually does not contain any
     *         attributes. This corresponds to the default text format.
     */
    public static AttributeSet getDefaultFormat() {
        return new SimpleAttributeSet();
    }
    
    /**
     * @return An {@code AttributeSet} that can be used to format error
     *         messages.
     */
    public static AttributeSet getErrorFormat() {
        SimpleAttributeSet format = new SimpleAttributeSet();
        StyleConstants.setForeground(format, Color.RED);
        StyleConstants.setBold(format, true);
        return format;
    }
    
    /**
     * @return An {@code AttributeSet} that can be used to format warning
     *         messages.
     */
    public static AttributeSet getWarningFormat() {
        SimpleAttributeSet format = new SimpleAttributeSet();
        StyleConstants.setForeground(format, Color.YELLOW);
        StyleConstants.setBold(format, true);
        return format;
    }
    
    /**
     * @return An {@code AttributeSet} that can be used to format informative
     *         messages.
     */
    public static AttributeSet getInfoFormat() {
        SimpleAttributeSet format = new SimpleAttributeSet();
        StyleConstants.setForeground(format, Color.CYAN);
        return format;
    }
    
    /**
     * @return An {@code AttributeSet} that can be used for private messages
     *         from other users.
     */
    public static AttributeSet getPMFormat() {
        SimpleAttributeSet format = new SimpleAttributeSet();
        StyleConstants.setForeground(format, Color.ORANGE);
        return format;
    }
    
    /** @return An {@code AttributeSet} that can be used to format the own input. */
    public static AttributeSet getOwnInputFormat() {
        SimpleAttributeSet format = new SimpleAttributeSet();
        StyleConstants.setForeground(format, Color.GRAY);
        return format;
    }
    
    /**
     * @return An {@code AttributeSet} that can be used to format error messages
     *         from the server.
     */
    public static AttributeSet getServerErrorFormat() {
        SimpleAttributeSet format = new SimpleAttributeSet();
        StyleConstants.setForeground(format, Color.RED);
        return format;
    }
}
