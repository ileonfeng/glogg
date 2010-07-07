/*
 * Copyright (C) 2010 Nicolas Bonnefon and other contributors
 *
 * This file is part of glogg.
 *
 * glogg is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * glogg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with glogg.  If not, see <http://www.gnu.org/licenses/>.
 */

// This file implements Selection.
// This class implements the selection handling. No check is made on
// the validity of the selection, it must be handled by the caller.
// There are three types of selection, only one type might be active
// at any time.

#include <QtGlobal>

#include "selection.h"

Selection::Selection()
{
    selectedLine_ = -1;

    selectedPartial_.line        = -1;
    selectedPartial_.startColumn = 0;
    selectedPartial_.endColumn   = 0;

    selectedRange_.startLine = -1;
    selectedRange_.endLine   = 0;
}

void Selection::selectPortion( int line, int start_column, int end_column )
{
    // First unselect any whole line or range
    selectedLine_ = -1;
    selectedRange_.startLine = -1;

    selectedPartial_.line = line;
    selectedPartial_.startColumn = qMin ( start_column, end_column );
    selectedPartial_.endColumn   = qMax ( start_column, end_column );
}

void Selection::selectRange( int start_line, int end_line )
{
    // First unselect any whole line and portion
    selectedLine_ = -1;
    selectedPartial_.line = -1;

    selectedRange_.startLine = qMin ( start_line, end_line );
    selectedRange_.endLine   = qMax ( start_line, end_line );
}

bool Selection::getPortionForLine( int line, int* start_column, int* end_column ) const
{
    if ( selectedPartial_.line == line ) {
        *start_column = selectedPartial_.startColumn;
        *end_column   = selectedPartial_.endColumn;

        return true;
    }
    else {
        return false;
    }
}

bool Selection::isLineSelected( int line ) const
{
    if ( line == selectedLine_ )
        return true;
    else if ( selectedRange_.startLine >= 0 )
        return ( ( line >= selectedRange_.startLine )
                && ( line <= selectedRange_.endLine ) );
    else
        return false;
}

QString Selection::getSelectedText( const AbstractLogData* logData ) const
{
    QString text;

    if ( selectedLine_ >= 0 ) {
        text = logData->getLineString( selectedLine_ );
    }
    else if ( selectedPartial_.line >= 0 ) {
        text = logData->getLineString( selectedPartial_.line ).
            mid( selectedPartial_.startColumn, ( selectedPartial_.endColumn -
                        selectedPartial_.startColumn ) + 1 );
    }
    else if ( selectedRange_.startLine >= 0 ) {
        QStringList list = logData->getLines( selectedRange_.startLine,
                selectedRange_.endLine - selectedRange_.startLine + 1 );
        text = list.join( "\n" );
    }

    return text;
}
