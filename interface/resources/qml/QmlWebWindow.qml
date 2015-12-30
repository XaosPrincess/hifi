import QtQuick 2.3
import QtQuick.Controls 1.2
import QtWebEngine 1.1

import "controls"
import "styles"

VrDialog {
    id: root
    HifiConstants { id: hifi }
    title: "WebWindow"
    resizable: true
    enabled: false
    visible: false
    // Don't destroy on close... otherwise the JS/C++ will have a dangling pointer
    destroyOnCloseButton: false
    contentImplicitWidth: clientArea.implicitWidth
    contentImplicitHeight: clientArea.implicitHeight
    backgroundColor: "#7f000000"
    property url source: "about:blank"

    signal navigating(string url)
    function stop() {
        webview.stop();
    }

    Component.onCompleted: {
        // Ensure the JS from the web-engine makes it to our logging
        webview.javaScriptConsoleMessage.connect(function(level, message, lineNumber, sourceID) {
            console.log("Web Window JS message: " + sourceID + " " + lineNumber + " " +  message);
        });

        // Required to support clicking on "hifi://" links
        webview.loadingChanged.connect(handleWebviewLoading) 
    }

    // Required to support clicking on "hifi://" links
    function handleWebviewLoading(loadRequest) {
        if (WebEngineView.LoadStartedStatus == loadRequest.status) {
            var newUrl = loadRequest.url.toString();
            root.navigating(newUrl)
        }
    }

    Item {
        id: clientArea
        implicitHeight: 600
        implicitWidth: 800
        x: root.clientX
        y: root.clientY
        width: root.clientWidth
        height: root.clientHeight

        WebEngineView {
            id: webview
            url: root.source
            anchors.fill: parent
            focus: true

            onUrlChanged: {
                var currentUrl = url.toString();
                var newUrl = urlFixer.fixupUrl(currentUrl);
                if (newUrl != currentUrl) {
                    url = newUrl;
                }
            }

            profile: WebEngineProfile {
                id: webviewProfile
                httpUserAgent: "Mozilla/5.0 (HighFidelityInterface)"
                storageName: "qmlWebEngine"
            } 
        }
    } // item
} // dialog
