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
#include <iostream>
#include "ShapesEditor.h"
#include "ShapeFusionApp.h"

IMPLEMENT_APP(ShapeFusionApp);

bool ShapeFusionApp::OnInit(void)
{
	ShapesEditor	*se = new ShapesEditor(wxT("ShapeFusion"), 10, 25, 900, 600);

	se->Show(true);
	SetTopWindow(se);
	return true;
}

