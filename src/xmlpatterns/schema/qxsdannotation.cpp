/***********************************************************************
*
* Copyright (c) 2012-2020 Barbara Geller
* Copyright (c) 2012-2020 Ansel Sermersheim
*
* Copyright (c) 2015 The Qt Company Ltd.
* Copyright (c) 2012-2016 Digia Plc and/or its subsidiary(-ies).
* Copyright (c) 2008-2012 Nokia Corporation and/or its subsidiary(-ies).
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
* https://www.gnu.org/licenses/
*
***********************************************************************/

#include "qxsdannotation_p.h"

QT_BEGIN_NAMESPACE

using namespace QPatternist;

void XsdAnnotation::setId(const DerivedString<TypeID>::Ptr &id)
{
   m_id = id;
}

DerivedString<TypeID>::Ptr XsdAnnotation::id() const
{
   return m_id;
}

void XsdAnnotation::addApplicationInformation(const XsdApplicationInformation::Ptr &information)
{
   m_applicationInformation.append(information);
}

XsdApplicationInformation::List XsdAnnotation::applicationInformation() const
{
   return m_applicationInformation;
}

void XsdAnnotation::addDocumentation(const XsdDocumentation::Ptr &documentation)
{
   m_documentations.append(documentation);
}

XsdDocumentation::List XsdAnnotation::documentation() const
{
   return m_documentations;
}

QT_END_NAMESPACE
