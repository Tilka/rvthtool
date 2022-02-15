/***************************************************************************
 * RVT-H Tool (qrvthtool)                                                  *
 * MessageSound.hpp: Message sound effects abstraction class.              *
 *                                                                         *
 * Copyright (c) 2018-2019 by David Korth.                                 *
 * SPDX-License-Identifier: GPL-2.0-or-later                               *
 ***************************************************************************/

#ifndef __RVTHTOOL_QRVTHTOOL_MESSAGESOUND_HPP__
#define __RVTHTOOL_QRVTHTOOL_MESSAGESOUND_HPP__

#include <QtWidgets/QMessageBox>

class MessageSound
{
	private:
		// MessageSound is a private class.
		MessageSound();
		~MessageSound();
		Q_DISABLE_COPY(MessageSound);

	public:
		/**
		 * Play a message sound effect.
		 * @param notificationType Notification type.
		 * @param message Message for logging. (not supported on all systems)
		 * @param parent Parent window. (not supported on all systems)
		 */
		static void play(QMessageBox::Icon notificationType, const QString &message = QString(), QWidget *parent = nullptr);
};

#endif /* __RVTHTOOL_QRVTHTOOL_MESSAGESOUND_HPP__ */
