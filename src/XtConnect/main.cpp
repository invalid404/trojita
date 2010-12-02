/*
    Certain enhancements (www.xtuple.com/trojita-enhancements)
    are copyright © 2010 by OpenMFG LLC, dba xTuple.  All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    - Redistributions of source code must retain the above copyright notice, this
    list of conditions and the following disclaimer.
    - Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.
    - Neither the name of xTuple nor the names of its contributors may be used to
    endorse or promote products derived from this software without specific prior
    written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
    ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
    ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/
#include <QCoreApplication>
#include <QSettings>
#include "XtConnect.h"

#ifdef HAS_GITVERSION
extern const char* gitVersion;
#endif

int main( int argc, char** argv) {
    QCoreApplication app( argc, argv );
    // Don't forget to update src/Gui/SettingsDialog.cpp if these names/domains/organizations change!
    QCoreApplication::setApplicationName( QString::fromAscii("xtconnect-trojita") );
#ifdef HAS_GITVERSION
    QCoreApplication::setApplicationVersion( QString::fromAscii( gitVersion ) );
#else
    QCoreApplication::setApplicationVersion( QString::fromAscii("0.2") );
#endif
    QCoreApplication::setOrganizationDomain( QString::fromAscii("xtuple.com") );
    QCoreApplication::setOrganizationName( QString::fromAscii("xtuple.com") );
    QSettings s;
    XtConnect::XtConnect conn(0, &s);
    return app.exec();
}