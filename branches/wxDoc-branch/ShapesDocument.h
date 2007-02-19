/*
 * This file is part of ShapeFusion (Copyright 2000 Tito Dal Canton)
 *
 * ShapeFusion is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * ShapeFusion is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ShapeFusion; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef __TEXTDOCUMENT_H__
#define __TEXTDOCUMENT_H__

#include "wx/docview.h"

class ShapesDocument: public wxDocument
{
    DECLARE_DYNAMIC_CLASS(ShapesDocument)
private:
public:

#if wxUSE_STD_IOSTREAM
    wxSTD ostream& SaveObject(wxSTD ostream& text_stream);
    wxSTD istream& LoadObject(wxSTD istream& text_stream);
#else
    wxOutputStream& SaveObject(wxOutputStream& stream);
    wxInputStream& LoadObject(wxInputStream& stream);
#endif

    ShapesDocument(void) {}
    ~ShapesDocument(void) {}
};

#endif