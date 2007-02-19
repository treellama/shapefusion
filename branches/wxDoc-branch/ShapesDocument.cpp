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

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif
#ifdef __WXMAC__
#include "wx/filename.h"
#endif

#include "ShapesDocument.h"

#include "ShapesView.h"

IMPLEMENT_DYNAMIC_CLASS(ShapesDocument, wxDocument)

#if wxUSE_STD_IOSTREAM
wxSTD ostream& ShapesDocument::SaveObject(wxSTD ostream& text_stream)
{
	return stream;
}
	
wxSTD istream& ShapesDocument::LoadObject(wxSTD istream& text_stream)
{
	return stream;
}
#else
wxOutputStream& ShapesDocument::SaveObject(wxOutputStream& stream)
{
	return stream;
}
	
wxInputStream& ShapesDocument::LoadObject(wxInputStream& stream)
{
	return stream;
}
#endif