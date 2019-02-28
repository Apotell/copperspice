/***********************************************************************
*
* Copyright (c) 2012-2018 Barbara Geller
* Copyright (c) 2012-2018 Ansel Sermersheim
* Copyright (c) 2012-2016 Digia Plc and/or its subsidiary(-ies).
* Copyright (c) 2008-2012 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
*
* This file is part of CopperSpice.
*
* CopperSpice is free software. You can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public License
* version 2.1 as published by the Free Software Foundation.
*
* CopperSpice is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*
* <http://www.gnu.org/licenses/>.
*
***********************************************************************/

#ifndef QHARFBUZZ_GUI_P_H
#define QHARFBUZZ_GUI_P_H

#include <qharfbuzz_core_p.h>

#include <hb.h>
#include <hb-font.hh>
#include <hb-ot.h>

class QFontEngine;

// following define is required due to something in Harfbuzz
#undef round

using glyph_t = quint32;
using HB_Bool = hb_bool_t;

using qt_destroy_func_ptr        = void (*)(void *);
using qt_get_font_table_func_ptr = bool (*)(void *, uint, uchar *, uint *);

// Font
Q_GUI_EXPORT hb_font_funcs_t *cs_get_font_funcs();

Q_GUI_EXPORT hb_face_t *cs_face_get_for_engine(QFontEngine *fe);
Q_GUI_EXPORT hb_font_t *cs_font_get_for_engine(QFontEngine *fe);

Q_GUI_EXPORT void cs_font_set_use_design_metrics(hb_font_t *font, uint value);
Q_GUI_EXPORT uint cs_font_get_use_design_metrics(hb_font_t *font);

#endif