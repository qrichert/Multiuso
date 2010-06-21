/*

Copyright © 2010 Shydow (www.siteduzero.com)

Multiuso is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Multiuso is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Multiuso.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "HighlighterActionScript.h"

HighlighterActionScript::HighlighterActionScript(QTextDocument *parent = 0) : QSyntaxHighlighter(parent)
{
        RegleDeSurlignage regle;

        formatMotsCles.setForeground(Qt::darkBlue);
        formatMotsCles.setFontWeight(QFont::Bold);

        QStringList modelesMotsCle;
                modelesMotsCle << "include" << "for" << "foreach" << "each" << "if" << "elseif" << "else" << "while" << "do" << "dowhile" <<
                                "endwhile" << "endif" << "switch" << "case" << "endswitch" << "return" << "break" << "continue" << "in" <<
                                "null" << "false" << "true" << "var" << "default" << "function" << "class" << "new" << "_global" <<
                                "endinitclip" << "initclip" << "__proto__" << "_accProps" << "_alpha" << "_currentframe" <<
                                "_droptarget" << "_focusrect" << "_framesloaded" << "_height" << "_highquality" << "_lockroot" <<
                                "_name" << "_parent" << "_quality" << "_root" << "_rotation" << "_soundbuftime" << "_target" << "_totalframes" <<
                                "_url" << "_visible" << "_width" << "_x" << "_xmouse" << "_xscale" << "_y" << "_ymouse" << "_yscale" << "abs" <<
                                "Accessibility" << "acos" << "activityLevel" << "add" << "addListener" << "addPage" << "addProperty" <<
                                "addRequestHeader" << "align" << "allowDomain" << "allowInsecureDomain" << "and" << "appendChild" <<
                                "apply" << "Arguments" << "Array" << "asfunction" << "asin" << "atan" << "atan2" << "attachAudio" << "attachMovie" <<
                                "attachSound" << "attachVideo" << "attributes" << "autosize" << "avHardwareDisable" << "background" <<
                                "backgroundColor" << "BACKSPACE" << "bandwidth" << "beginFill" << "beginGradientFill" << "blockIndent" <<
                                "bold" << "Boolean" << "border" << "borderColor" << "bottomScroll" << "bufferLength" << "bufferTime" <<
                                "builtInItems" << "bullet" << "Button" << "bytesLoaded" << "bytesTotal" << "call" << "callee" << "caller" <<
                                "Camera" << "capabilities" << "CAPSLOCK" << "caption" << "catch" << "ceil" << "charAt" << "charCodeAt" <<
                                "childNodes" << "chr" << "clear" << "clearInterval" << "cloneNode" << "close" << "Color" << "concat" <<
                                "connect" << "condenseWhite" << "constructor" << "contentType" << "ContextMenu" << "ContextMenuItem" <<
                                "CONTROL" << "copy" << "cos" << "createElement" << "createEmptyMovieClip" << "createTextField" <<
                                "createTextNode" << "currentFps" << "curveTo" << "CustomActions" << "customItems" << "data" << "Date" <<
                                "deblocking" << "delete" << "DELETEKEY" << "docTypeDecl" << "domain" << "DOWN" <<
                                "duplicateMovieClip" << "duration" << "dynamic" << "E" << "embedFonts" << "enabled" <<
                                "END" << "endFill" << "ENTER" << "eq" << "Error" << "ESCAPE" << "escape" << "eval" <<
                                "exactSettings" << "exp" << "extends" << "finally" << "findText" << "firstChild" << "floor" <<
                                "flush" << "focusEnabled" << "font" << "fps" << "fromCharCode" << "fscommand" <<
                                "gain" << "ge" << "get" << "getAscii" << "getBeginIndex" << "getBounds" << "getBytesLoaded" << "getBytesTotal" <<
                                "getCaretIndex" << "getCode" << "getCount" << "getDate" << "getDay" << "getDepth" << "getEndIndex" << "getFocus" <<
                                "getFontList" << "getFullYear" << "getHours" << "getInstanceAtDepth" << "getLocal" << "getMilliseconds" <<
                                "getMinutes" << "getMonth" << "getNewTextFormat" << "getNextHighestDepth" << "getPan" << "getProgress" <<
                                "getProperty" << "getRGB" << "getSeconds" << "getSelected" << "getSelectedText" << "getSize" << "getStyle" <<
                                "getStyleNames" << "getSWFVersion" << "getText" << "getTextExtent" << "getTextFormat" << "getTextSnapshot" <<
                                "getTime" << "getTimer" << "getTimezoneOffset" << "getTransform" << "getURL" << "getUTCDate" << "getUTCDay" <<
                                "getUTCFullYear" << "getUTCHours" << "getUTCMilliseconds" << "getUTCMinutes" << "getUTCMonth" << "getUTCSeconds" <<
                                "getVersion" << "getVolume" << "getYear" << "globalToLocal" << "goto" << "gotoAndPlay" << "gotoAndStop" <<
                                "hasAccessibility" << "hasAudio" << "hasAudioEncoder" << "hasChildNodes" << "hasEmbeddedVideo" << "hasMP3" <<
                                "hasPrinting" << "hasScreenBroadcast" << "hasScreenPlayback" << "hasStreamingAudio" << "hasStreamingVideo" <<
                                "hasVideoEncoder" << "height" << "hide" << "hideBuiltInItems" << "hitArea" << "hitTest" << "hitTestTextNearPos" <<
                                "HOME" << "hscroll" << "html" << "htmlText" << "ID3" << "ifFrameLoaded" << "ignoreWhite" << "implements" <<
                                "import" << "indent" << "index" << "indexOf" << "Infinity" << "-Infinity" << "INSERT" << "insertBefore" << "install" <<
                                "instanceof" << "int" << "interface" << "isActive" << "isDebugger" << "isDown" << "isFinite" << "isNaN" << "isToggled" <<
                                "italic" << "join" << "Key" << "language" << "lastChild" << "lastIndexOf" << "le" << "leading" << "LEFT" << "leftMargin" <<
                                "length" << "level" << "lineStyle" << "lineTo" << "list" << "LN10" << "LN2" << "load" << "loadClip" << "loaded" << "loadMovie" <<
                                "loadMovieNum" << "loadSound" << "loadVariables" << "loadVariablesNum" << "LoadVars" << "LocalConnection" <<
                                "localFileReadDisable" << "localToGlobal" << "log" << "LOG10E" << "LOG2E" << "manufacturer" << "Math" << "max" <<
                                "MAX_VALUE" << "maxChars" << "maxhscroll" << "maxscroll" << "mbchr" << "mblength" << "mbord" << "mbsubstring" << "menu" <<
                                "message" << "Microphone" << "min" << "MIN_VALUE" << "MMExecute" << "motionLevel" << "motionTimeOut" << "Mouse" <<
                                "mouseWheelEnabled" << "moveTo" << "Movieclip" << "MovieClipLoader" << "multiline" << "muted" << "name" << "names" << "NaN" <<
                                "ne" << "NEGATIVE_INFINITY" << "NetConnection" << "NetStream" << "newline" << "nextFrame" <<
                                "nextScene" << "nextSibling" << "nodeName" << "nodeType" << "nodeValue" << "not" << "Number" << "Object" <<
                                "on" << "onActivity" << "onChanged" << "onClipEvent" << "onClose" << "onConnect" << "onData" << "onDragOut" <<
                                "onDragOver" << "onEnterFrame" << "onID3" << "onKeyDown" << "onKeyUp" << "onKillFocus" << "onLoad" << "onLoadComplete" <<
                                "onLoadError" << "onLoadInit" << "onLoadProgress" << "onLoadStart" << "onMouseDown" << "onMouseMove" << "onMouseUp" <<
                                "onMouseWheel" << "onPress" << "onRelease" << "onReleaseOutside" << "onResize" << "onRollOut" << "onRollOver" <<
                                "onScroller" << "onSelect" << "onSetFocus" << "onSoundComplete" << "onStatus" << "onUnload" << "onUpdate" << "onXML" <<
                                "or" << "ord" << "os" << "parentNode" << "parseCSS" << "parseFloat" << "parseInt" << "parseXML" << "password" <<
                                "pause" << "PGDN" << "PGUP" << "PI" << "pixelAspectRatio" << "play" << "playerType" << "pop" << "position" <<
                                "POSITIVE_INFINITY" << "pow" << "prevFrame" << "previousSibling" << "prevScene" << "print" << "printAsBitmap" <<
                                "printAsBitmapNum" << "PrintJob" << "printNum" << "private" << "prototype" << "public" << "push" << "quality" <<
                                "random" << "rate" << "registerClass" << "removeListener" << "removeMovieClip" << "removeNode" << "removeTextField" <<
                                "replaceSel" << "replaceText" << "resolutionX" << "resolutionY" << "restrict" << "reverse" << "RIGHT" <<
                                "rightMargin" << "round" << "scaleMode" << "screenColor" << "screenDPI" << "screenResolutionX" << "screenResolutionY" <<
                                "scroll" << "seek" << "selectable" << "Selection" << "send" << "sendAndLoad" << "separatorBefore" << "serverString" <<
                                "set" << "setvariable" << "setBufferTime" << "setClipboard" << "setDate" << "setFocus" << "setFullYear" << "setGain" <<
                                "setHours" << "setInterval" << "setMask" << "setMilliseconds" << "setMinutes" << "setMode" << "setMonth" <<
                                "setMotionLevel" << "setNewTextFormat" << "setPan" << "setProperty" << "setQuality" << "setRate" << "setRGB" <<
                                "setSeconds" << "setSelectColor" << "setSelected" << "setSelection" << "setSilenceLevel" << "setStyle" <<
                                "setTextFormat" << "setTime" << "setTransform" << "setUseEchoSuppression" << "setUTCDate" << "setUTCFullYear" <<
                                "setUTCHours" << "setUTCMilliseconds" << "setUTCMinutes" << "setUTCMonth" << "setUTCSeconds" << "setVolume" <<
                                "setYear" << "SharedObject" << "SHIFT" << "shift" << "show" << "showMenu" << "showSettings" <<
                                "silenceLevel" << "silenceTimeout" << "sin" << "size" << "slice" << "smoothing" << "sort" << "sortOn" << "Sound" << "SPACE" <<
                                "splice" << "split" << "sqrt" << "SQRT1_2" << "SQRT2" << "Stage" << "start" << "startDrag" << "static" << "status" << "stop" <<
                                "stopAllSounds" << "stopDrag" << "String" << "StyleSheet" << "styleSheet" << "substr" <<
                                "substring" << "super" << "swapDepths" << "System" << "TAB" << "tabChildren" << "tabEnabled" << "tabIndex" <<
                                "tabStops" << "tan" << "target" << "targetPath" << "tellTarget" << "text" << "textColor" << "TextField" << "TextFormat" <<
                                "textHeight" << "TextSnapshot" << "textWidth" << "this" << "throw" << "time" << "toggleHighQuality" << "toLowerCase" <<
                                "toString" << "toUpperCase" << "trace" << "trackAsMenu" << "try" << "type" << "typeof" << "undefined" <<
                                "underline" << "unescape" << "uninstall" << "unloadClip" << "unloadMovie" << "unLoadMovieNum" << "unshift" << "unwatch" <<
                                "UP" << "updateAfterEvent" << "updateProperties" << "url" << "useCodePage" << "useEchoSuppression" << "useHandCursor" <<
                                "UTC" << "valueOf" << "variable" << "version" << "Video" << "visible" << "void" << "watch" << "width" <<
                                "with" << "wordwrap" << "XML" << "xmlDecl" << "XMLNode" << "XMLSocket";

        foreach (const QString &modele, modelesMotsCle)
        {
            regle.modele = QRegExp(modele);
            regle.format = formatMotsCles;

            reglesDeSurlignage << regle;
        }

        formatCommentairesMultiLigne.setForeground(Qt::darkGray);
        formatCommentairesMultiLigne.setFontItalic(true);

        formatNombres.setForeground(Qt::magenta);
                regle.modele = QRegExp("[0-9]+");
                regle.format = formatNombres;
                        reglesDeSurlignage << regle;

        formatDeQuotationSimple.setForeground(Qt::darkYellow);
            regle.modele = QRegExp("'.*'");
            regle.format = formatDeQuotationSimple;
                reglesDeSurlignage << regle;

        formatDeQuotationDouble.setForeground(Qt::darkYellow);
            regle.modele = QRegExp("\".*\"");
            regle.format = formatDeQuotationDouble;
                reglesDeSurlignage << regle;

        formatCommentairesUneLigne.setForeground(Qt::gray);
        formatCommentairesUneLigne.setFontItalic(true);
                regle.modele = QRegExp("(//|\\#)[^\n]*");
                regle.format = formatCommentairesUneLigne;
                        reglesDeSurlignage << regle;

        debutCommentaire = QRegExp("/\\*");
        finCommentaire = QRegExp("\\*/");
}

void HighlighterActionScript::highlightBlock(const QString &texte)
{
        foreach (const RegleDeSurlignage &regle, reglesDeSurlignage)
        {
                QRegExp expression(regle.modele);

                int index = expression.indexIn(texte);

                while (index >= 0)
                {
                        int longueur = expression.matchedLength();
                        setFormat(index, longueur, regle.format);
                        index = expression.indexIn(texte, index + longueur);
                }
        }

        setCurrentBlockState(0);

        int indexDebut = 0;

        if (previousBlockState() != 1)
        {
                indexDebut = debutCommentaire.indexIn(texte);
        }

        while (indexDebut >= 0)
        {
                int indexFin = finCommentaire.indexIn(texte, indexDebut);
                int longueurCommentaire;

                if (indexFin == -1)
                {
                        setCurrentBlockState(1);
                        longueurCommentaire = texte.length() - indexDebut;
                }

                else
                {
                        longueurCommentaire = indexFin - indexDebut + finCommentaire.matchedLength();
                }

                setFormat(indexDebut, longueurCommentaire, formatCommentairesMultiLigne);
                indexDebut = debutCommentaire.indexIn(texte, indexDebut + longueurCommentaire);
        }
}
