#include "ElaSvgIconPrivate.h"

#include "ElaSvgIcon.h"
ElaSvgIconPrivate::ElaSvgIconPrivate(QObject* parent)
    : QObject{parent}
{
    _svgMap[SvgIconType::UP] = "Up";
    _svgMap[SvgIconType::ADD] = "Add";
    _svgMap[SvgIconType::BUS] = "Bus";
    _svgMap[SvgIconType::CAR] = "Car";
    _svgMap[SvgIconType::CUT] = "Cut";
    _svgMap[SvgIconType::IOT] = "IOT";
    _svgMap[SvgIconType::PIN] = "Pin";
    _svgMap[SvgIconType::TAG] = "Tag";
    _svgMap[SvgIconType::VPN] = "VPN";
    _svgMap[SvgIconType::CAFE] = "Cafe";
    _svgMap[SvgIconType::CHAT] = "Chat";
    _svgMap[SvgIconType::COPY] = "Copy";
    _svgMap[SvgIconType::CODE] = "Code";
    _svgMap[SvgIconType::DOWN] = "Down";
    _svgMap[SvgIconType::EDIT] = "Edit";
    _svgMap[SvgIconType::FLAG] = "Flag";
    _svgMap[SvgIconType::FONT] = "Font";
    _svgMap[SvgIconType::GAME] = "Game";
    _svgMap[SvgIconType::HELP] = "Help";
    _svgMap[SvgIconType::HIDE] = "Hide";
    _svgMap[SvgIconType::HOME] = "Home";
    _svgMap[SvgIconType::INFO] = "Info";
    _svgMap[SvgIconType::LEAF] = "Leaf";
    _svgMap[SvgIconType::LINK] = "Link";
    _svgMap[SvgIconType::MAIL] = "Mail";
    _svgMap[SvgIconType::MENU] = "Menu";
    _svgMap[SvgIconType::MUTE] = "Mute";
    _svgMap[SvgIconType::MORE] = "More";
    _svgMap[SvgIconType::MOVE] = "Move";
    _svgMap[SvgIconType::PLAY] = "Play";
    _svgMap[SvgIconType::SAVE] = "Save";
    _svgMap[SvgIconType::SEND] = "Send";
    _svgMap[SvgIconType::SYNC] = "Sync";
    _svgMap[SvgIconType::UNIT] = "Unit";
    _svgMap[SvgIconType::VIEW] = "View";
    _svgMap[SvgIconType::WIFI] = "Wifi";
    _svgMap[SvgIconType::ZOOM] = "Zoom";
    _svgMap[SvgIconType::ALBUM] = "Album";
    _svgMap[SvgIconType::BRUSH] = "Brush";
    _svgMap[SvgIconType::BROOM] = "Broom";
    _svgMap[SvgIconType::CLOSE] = "Close";
    _svgMap[SvgIconType::CLOUD] = "Cloud";
    _svgMap[SvgIconType::EMBED] = "Embed";
    _svgMap[SvgIconType::GLOBE] = "Globe";
    _svgMap[SvgIconType::HEART] = "Heart";
    _svgMap[SvgIconType::LABEL] = "Label";
    _svgMap[SvgIconType::MEDIA] = "Media";
    _svgMap[SvgIconType::MOVIE] = "Movie";
    _svgMap[SvgIconType::MUSIC] = "Music";
    _svgMap[SvgIconType::ROBOT] = "Robot";
    _svgMap[SvgIconType::PAUSE] = "Pause";
    _svgMap[SvgIconType::PASTE] = "Paste";
    _svgMap[SvgIconType::PHOTO] = "Photo";
    _svgMap[SvgIconType::PHONE] = "Phone";
    _svgMap[SvgIconType::PRINT] = "Print";
    _svgMap[SvgIconType::SHARE] = "Share";
    _svgMap[SvgIconType::TILES] = "Tiles";
    _svgMap[SvgIconType::UNPIN] = "Unpin";
    _svgMap[SvgIconType::VIDEO] = "Video";
    _svgMap[SvgIconType::TRAIN] = "Train";
    _svgMap[SvgIconType::ADD_TO ] =";AddTo";
    _svgMap[SvgIconType::ACCEPT] = "Accept";
    _svgMap[SvgIconType::CAMERA] = "Camera";
    _svgMap[SvgIconType::CANCEL] = "Cancel";
    _svgMap[SvgIconType::DELETEE] = "Delete";
    _svgMap[SvgIconType::FOLDER] = "Folder";
    _svgMap[SvgIconType::FILTER] = "Filter";
    _svgMap[SvgIconType::MARKET] = "Market";
    _svgMap[SvgIconType::SCROLL] = "Scroll";
    _svgMap[SvgIconType::LAYOUT] = "Layout";
    _svgMap[SvgIconType::GITHUB] = "GitHub";
    _svgMap[SvgIconType::UPDATE] = "Update";
    _svgMap[SvgIconType::REMOVE] = "Remove";
    _svgMap[SvgIconType::RETURN] = "Return";
    _svgMap[SvgIconType::PEOPLE] = "People";
    _svgMap[SvgIconType::QRCODE] = "QRCode";
    _svgMap[SvgIconType::RINGER] = "Ringer";
    _svgMap[SvgIconType::ROTATE] = "Rotate";
    _svgMap[SvgIconType::SEARCH] = "Search";
    _svgMap[SvgIconType::VOLUME] = "Volume";
    _svgMap[SvgIconType::FRIGID ] = "Frigid";
    _svgMap[SvgIconType::SAVE_AS] = "SaveAs";
    _svgMap[SvgIconType::ZOOM_IN] = "ZoomIn";
    _svgMap[SvgIconType::CONNECT ] =";Connect";
    _svgMap[SvgIconType::HISTORY] = "History";
    _svgMap[SvgIconType::SETTING] = "Setting";
    _svgMap[SvgIconType::PALETTE] = "Palette";
    _svgMap[SvgIconType::MESSAGE] = "Message";
    _svgMap[SvgIconType::FIT_PAGE] = "FitPage";
    _svgMap[SvgIconType::ZOOM_OUT] = "ZoomOut";
    _svgMap[SvgIconType::AIRPLANE] = "Airplane";
    _svgMap[SvgIconType::ASTERISK] = "Asterisk";
    _svgMap[SvgIconType::CALORIES] = "Calories";
    _svgMap[SvgIconType::CALENDAR] = "Calendar";
    _svgMap[SvgIconType::FEEDBACK] = "Feedback";
    _svgMap[SvgIconType::LIBRARY] = "BookShelf";
    _svgMap[SvgIconType::MINIMIZE] = "Minimize";
    _svgMap[SvgIconType::CHECKBOX] = "CheckBox";
    _svgMap[SvgIconType::DOCUMENT] = "Document";
    _svgMap[SvgIconType::LANGUAGE] = "Language";
    _svgMap[SvgIconType::DOWNLOAD] = "Download";
    _svgMap[SvgIconType::QUESTION] = "Question";
    _svgMap[SvgIconType::SPEAKERS] = "Speakers";
    _svgMap[SvgIconType::DATE_TIME] = "DateTime";
    _svgMap[SvgIconType::FONT_SIZE] = "FontSize";
    _svgMap[SvgIconType::HOME_FILL] = "HomeFill";
    _svgMap[SvgIconType::PAGE_LEFT] = "PageLeft";
    _svgMap[SvgIconType::SAVE_COPY] = "SaveCopy";
    _svgMap[SvgIconType::SEND_FILL] = "SendFill";
    _svgMap[SvgIconType::SKIP_BACK] = "SkipBack";
    _svgMap[SvgIconType::SPEED_OFF] = "SpeedOff";
    _svgMap[SvgIconType::ALIGNMENT] = "Alignment";
    _svgMap[SvgIconType::BLUETOOTH] = "Bluetooth";
    _svgMap[SvgIconType::COMPLETED] = "Completed";
    _svgMap[SvgIconType::CONSTRACT] = "Constract";
    _svgMap[SvgIconType::HEADPHONE] = "Headphone";
    _svgMap[SvgIconType::MEGAPHONE] = "Megaphone";
    _svgMap[SvgIconType::PROJECTOR] = "Projector";
    _svgMap[SvgIconType::EDUCATION] = "Education";
    _svgMap[SvgIconType::LEFT_ARROW] = "LeftArrow";
    _svgMap[SvgIconType::ERASE_TOOL] = "EraseTool";
    _svgMap[SvgIconType::PAGE_RIGHT] = "PageRight";
    _svgMap[SvgIconType::PLAY_SOLID] = "PlaySolid";
    _svgMap[SvgIconType::BOOK_SHELF] = "BookShelf";
    _svgMap[SvgIconType::HIGHTLIGHT] = "Highlight";
    _svgMap[SvgIconType::FOLDER_ADD] = "FolderAdd";
    _svgMap[SvgIconType::PAUSE_BOLD] = "PauseBold";
    _svgMap[SvgIconType::PENCIL_INK] = "PencilInk";
    _svgMap[SvgIconType::PIE_SINGLE] = "PieSingle";
    _svgMap[SvgIconType::QUICK_NOTE] = "QuickNote";
    _svgMap[SvgIconType::SPEED_HIGH] = "SpeedHigh";
    _svgMap[SvgIconType::STOP_WATCH] = "StopWatch";
    _svgMap[SvgIconType::ZIP_FOLDER] = "ZipFolder";
    _svgMap[SvgIconType::BASKETBALL] = "Basketball";
    _svgMap[SvgIconType::BRIGHTNESS] = "Brightness";
    _svgMap[SvgIconType::DICTIONARY] = "Dictionary";
    _svgMap[SvgIconType::MICROPHONE] = "Microphone";
    _svgMap[SvgIconType::ARROW_DOWN] = "ChevronDown";
    _svgMap[SvgIconType::FULL_SCREEN] = "FullScreen";
    _svgMap[SvgIconType::MIX_VOLUMES] = "MixVolumes";
    _svgMap[SvgIconType::REMOVE_FROM] = "RemoveFrom";
    _svgMap[SvgIconType::RIGHT_ARROW] = "RightArrow";
    _svgMap[SvgIconType::QUIET_HOURS ] =";QuietHours";
    _svgMap[SvgIconType::FINGERPRINT] = "Fingerprint";
    _svgMap[SvgIconType::APPLICATION] = "Application";
    _svgMap[SvgIconType::CERTIFICATE] = "Certificate";
    _svgMap[SvgIconType::TRANSPARENTE] = "Transparent";
    _svgMap[SvgIconType::IMAGE_EXPORT] = "ImageExport";
    _svgMap[SvgIconType::SPEED_MEDIUM] = "SpeedMedium";
    _svgMap[SvgIconType::LIBRARY_FILL] = "LibraryFill";
    _svgMap[SvgIconType::MUSIC_FOLDER] = "MusicFolder";
    _svgMap[SvgIconType::POWER_BUTTON] = "PowerButton";
    _svgMap[SvgIconType::SKIP_FORWARD] = "SkipForward";
    _svgMap[SvgIconType::CARE_UP_SOLID] = "CareUpSolid";
    _svgMap[SvgIconType::ACCEPT_MEDIUM] = "AcceptMedium";
    _svgMap[SvgIconType::CANCEL_MEDIUM] = "CancelMedium";
    _svgMap[SvgIconType::CHEVRON_RIGHT] = "ChevronRight";
    _svgMap[SvgIconType::CLIPPING_TOOL] = "ClippingTool";
    _svgMap[SvgIconType::SEARCH_MIRROR] = "SearchMirror";
    _svgMap[SvgIconType::SHOPPING_CART] = "ShoppingCart";
    _svgMap[SvgIconType::FONT_INCREASE] = "FontIncrease";
    _svgMap[SvgIconType::BACK_TO_WINDOW] = "BackToWindow";
    _svgMap[SvgIconType::COMMAND_PROMPT] = "CommandPrompt";
    _svgMap[SvgIconType::CLOUD_DOWNLOAD] = "CloudDownload";
    _svgMap[SvgIconType::DICTIONARY_ADD] = "DictionaryAdd";
    _svgMap[SvgIconType::CARE_DOWN_SOLID] = "CareDownSolid";
    _svgMap[SvgIconType::CARE_LEFT_SOLID] = "CareLeftSolid";
    _svgMap[SvgIconType::CLEAR_SELECTION] = "ClearSelection";
    _svgMap[SvgIconType::DEVELOPER_TOOLS] = "DeveloperTools";
    _svgMap[SvgIconType::BACKGROUND_FILL] = "BackgroundColor";
    _svgMap[SvgIconType::CARE_RIGHT_SOLID] = "CareRightSolid";
    _svgMap[SvgIconType::CHEVRON_DOWN_MED] = "ChevronDownMed";
    _svgMap[SvgIconType::CHEVRON_RIGHT_MED] = "ChevronRightMed";
    _svgMap[SvgIconType::EMOJI_TAB_SYMBOLS] = "EmojiTabSymbols";
    _svgMap[SvgIconType::EXPRESSIVE_INPUT_ENTRY] = "ExpressiveInputEntry";
}

ElaSvgIconPrivate::~ElaSvgIconPrivate()
{
}

void ElaSvgIconPrivate::updateIcon(SvgIconType::IconName icon, int iconwidget, int iconheight)
{
    if (icon != SvgIconType::NONE) {
        _iconName = icon;
    }
    if (_pixmap.isNull()) {
        _pixmap = QPixmap(iconwidget*2, iconheight*2);
    }
    static QSvgRenderer renderer;
    renderer.load(QString(":/include/Image/icons/%1_%2.svg")
                      .arg(_svgMap[_iconName]).arg(_themeMode == ElaThemeType::Light ? "black" : "white"));
    _pixmap.fill(Qt::transparent);
    if (iconheight*iconwidget)
        _pixmap = _pixmap.scaled(iconwidget*2, iconheight*2);
    QPainter painter(&_pixmap);
    renderer.render(&painter);
}
