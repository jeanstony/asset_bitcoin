// Copyright (c) 2018 The Vccoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <qt/VCcoin.h>

#include <QCoreApplication>

#include <functional>
#include <string>
#include <qt/guiutil.h>

/** Translate string to current locale using Qt. */
extern const std::function<std::string(const char*)> G_TRANSLATION_FUN = [](const char* psz) {
    return GUIUtil::toStdString(QCoreApplication::translate("VCcoin-core", psz));
};

int main(int argc, char* argv[]) {
	//qputenv("QT_OPENGL", "angle");
    return GuiMain(argc, argv);
}
