/*
 * SPDX-License-Identifier: GPL-3.0-only
 * MuseScore-Studio-CLA-applies
 *
 * MuseScore Studio
 * Music Composition & Notation
 *
 * Copyright (C) 2022 MuseScore Limited
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "loadingscreenview.h"

#include <QApplication>
#include <QPainter>
#include <QScreen>
#include <QSvgRenderer>

#include "translation.h"

using namespace mu::appshell;

static const QString imagePath(":/resources/LoadingScreen.svg");

static constexpr QSize loadingScreenSize(810, 406);

static const QColor messageColor("#F1F1EE");
static constexpr QRectF messageRect(48, 230, 0, 0);

static const QString website("www.musescore.org");
static constexpr QRectF websiteRect(loadingScreenSize.width() - 48, loadingScreenSize.height() - 48, 0, 0);

static const QColor versionNumberColor("#19F3FF");
static constexpr qreal versionNumberSpacing = 5.0;

LoadingScreenView::LoadingScreenView(QWidget* parent)
    : QWidget(parent), muse::Injectable(muse::iocCtxForQWidget(this)),
    m_backgroundRenderer(new QSvgRenderer(imagePath, this))
{
    setAttribute(Qt::WA_TranslucentBackground);
    resize(loadingScreenSize);

    m_message = muse::qtrc("appshell", "Loading…\u200e");
}

bool LoadingScreenView::event(QEvent* event)
{
    if (event->type() == QEvent::Paint) {
        QPainter painter(this);
        painter.setLayoutDirection(layoutDirection());
        draw(&painter);
    }

    return QWidget::event(event);
}

void LoadingScreenView::draw(QPainter* painter)
{
    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);

    // Draw background
    m_backgroundRenderer->render(painter);

    // Draw message
    QFont font(QString::fromStdString(uiConfiguration()->fontFamily()));
    font.setPixelSize(uiConfiguration()->fontSize());

    painter->setFont(font);

    QPen pen(messageColor);
    painter->setPen(pen);

    painter->drawText(messageRect, Qt::AlignTop | Qt::AlignLeft | Qt::TextDontClip, m_message);

    Qt::AlignmentFlag alignment = languagesService()->currentLanguage().direction == Qt::RightToLeft
                                  ? Qt::AlignLeft : Qt::AlignRight;

    // Draw website URL
    QRectF websiteBoundingRect;
    painter->drawText(websiteRect, Qt::AlignBottom | alignment | Qt::TextDontClip, website, &websiteBoundingRect);

    // Draw version number
    pen.setColor(versionNumberColor);
    painter->setPen(pen);

    painter->drawText(websiteRect.translated(0.0, -websiteBoundingRect.height() - versionNumberSpacing),
                      Qt::AlignBottom | alignment | Qt::TextDontClip,
                      muse::qtrc("appshell", "Version %1").arg(application()->fullVersion().toString()));
}
