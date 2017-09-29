// Constants.js
.pragma library

// Program options
var USE_SDK_PIN_UI_POPUP = true

// Error Constants
var TRIES_LEFT_ERROR = 1000

// Certificates Status
var PTEID_CERTIF_STATUS_UNKNOWN = 0     /* Validity unknown */
var PTEID_CERTIF_STATUS_REVOKED = 1     /* Revoked certificate */
var PTEID_CERTIF_STATUS_SUSPENDED = 2
var PTEID_CERTIF_STATUS_CONNECT = 3     /* Connection problem */
var PTEID_CERTIF_STATUS_ISSUER = 4      /* An issuer is missing in the chain */
var PTEID_CERTIF_STATUS_ERROR = 5       /* Error during validation */
var PTEID_CERTIF_STATUS_VALID = 6       /* Valid certificate */

// Screen Size
var SCREEN_MINIMUM_WIDTH = 1024
var SCREEN_MINIMUM_HEIGHT = 576;

// Title bar
var TITLE_BAR_SIZE = 25
var TITLE_BAR_H_ICON_SPACE = 30

// Frame detection
var FRAME_WINDOW_SIZE = 20

// Views Relative Size
var MAIN_MENU_VIEW_RELATIVE_SIZE = 0.25
var SUB_MENU_VIEW_RELATIVE_SIZE = 0.25
var CONTENT_PAGES_VIEW_RELATIVE_SIZE = 0.50

// Main Menu Size
var BOTTOM_MENU_WIDTH_SIZE = 100
var IMAGE_LOGO_RELATIVE_V_POS = 0.05
var MAIN_MENU_RELATIVE_V_SIZE = 0.40
var MAIN_MENU_BOTTOM_RELATIVE_V_SIZE = 0.05
var MAIN_MENU_BOTTOM_RELATIVE_V_POS = 0.85
var MAIN_MENU_LINE_H_SIZE = 70
var MAIN_MENU_LINE_V_SIZE = 1
var IMAGE_ARROW_MAIN_MENU_RELATIVE = 0.9

// Sub Menu Size
var SUB_MENU_RELATIVE_V_ITEM_SIZE = 0.15
var SUB_MENU_RELATIVE_LINE_H_SIZE = 0.005
var IMAGE_ARROW_SUB_MENU_MARGIN = 5

// Text Size
var SIZE_TEXT_MAIN_MENU = 16
var SIZE_TEXT_SUB_MENU = 16
var SIZE_TEXT_TITLE = 20
var SIZE_TEXT_LABEL = 12
var SIZE_TEXT_FIELD = 12
var SIZE_TEXT_BODY = 16
var SIZE_TEXT_LIST_BULLET = 8

// Buttons Size
var WIDTH_BUTTON = 150
var HEIGHT_BOTTOM_COMPONENT = 35
var HEIGHT_SIGN_BOTTOM_COMPONENT = 54

// Switch Size
var HEIGHT_SWITCH_COMPONENT = 30

// RadioButton Size
var HEIGHT_RADIO_BOTTOM_COMPONENT = 30

// Defined Colors
var COLOR_MAIN_BLUE = "#3C5DBC"
var COLOR_MAIN_PRETO = "#333333"
var COLOR_MAIN_SOFT_GRAY = "#EDEDED"
var COLOR_MAIN_DARK_GRAY = "#9FAFDF"
var COLOR_GREY_BUTTON_BACKGROUND = "#D6D7D7"

// Menus Text Colors
var COLOR_TEXT_MAIN_MENU_SELECTED = COLOR_MAIN_SOFT_GRAY
var COLOR_TEXT_MAIN_MENU_DEFAULT = COLOR_MAIN_DARK_GRAY
var COLOR_TEXT_SUB_MENU_SELECTED = COLOR_MAIN_SOFT_GRAY
var COLOR_TEXT_SUB_MENU_DEFAULT = COLOR_MAIN_PRETO
var COLOR_TEXT_SUB_MENU_MOUSE_OVER = COLOR_MAIN_BLUE
var COLOR_TEXT_TITLE = COLOR_MAIN_BLUE

// Colors
var COLOR_BACKGROUND_MAIN_MENU = COLOR_MAIN_BLUE
var COLOR_BACKGROUND_SUB_MENU = COLOR_MAIN_BLUE
var COLOR_LINE_SUB_MENU = COLOR_MAIN_SOFT_GRAY
var COLOR_TEXT_BODY = COLOR_MAIN_PRETO
var COLOR_TEXT_LABEL = COLOR_MAIN_BLUE
var COLOR_ITEM_BULLETED = COLOR_MAIN_BLUE

// Forms
// Shadow
var FORM_SHADOW_OPACITY_FORM_EFFECT = 0.15
var COLOR_FORM_SHADOW = "#000000"
var FORM_SHADOW_H_OFFSET = 1.4
var FORM_SHADOW_V_OFFSET = 1.4
var FORM_SHADOW_RADIUS = 6.0
var FORM_SHADOW_SAMPLES = 20
var FORM_SHADOW_SPREAD = 0.0
// Glow
var FORM_GLOW_OPACITY_FORM_EFFECT = 0.15
var COLOR_FORM_GLOW = "#000000"
var FORM_GLOW_RADIUS = 2
var FORM_GLOW_CORNER_RADIUS = 0
var FORM_GLOW_SPREAD = 0

// Text space
var SIZE_TEXT_FIELD_H_SPACE = 10
var SIZE_TEXT_FIELD_V_SPACE = 4
var SIZE_TEXT_V_SPACE = 5

// Row space
var SIZE_ROW_H_SPACE = 20
var SIZE_ROW_V_SPACE = 12
var SIZE_ROW_V_SPACE_DIFINITIONS_APP = 7

// Animation time
var ANIMATION_MOVE_VIEW = 400
var ANIMATION_CHANGE_OPACITY = 300
var ANIMATION_LISTVIEW_MOVE = 400
var ANIMATION_LISTVIEW_RESIZE = 400

// Image Constants
var SIZE_IMAGE_LOGO_MAIN_MENU_WIDTH = 89
var SIZE_IMAGE_LOGO_MAIN_MENU_HEIGHT = 100
var SIZE_IMAGE_LOGO_CC_WIDTH = 160
var SIZE_IMAGE_LOGO_CC_HEIGHT = 35
var SIZE_IMAGE_ICON_TITLE_BAR= 15
var SIZE_IMAGE_ARROW_ACCORDION = 12
var SIZE_IMAGE_ARROW_SUB_MENU = 16
var SIZE_IMAGE_ARROW_MAIN_MENU = 16
var SIZE_IMAGE_BOTTOM_MENU = 20
var SIZE_IMAGE_FILE_REMOVE = 20
var SIZE_IMAGE_TOOLTIP = 15

// Pages Constants
// Home Page
var HEIGHT_HOME_PAGE_ROW_TOP_V_RELATIVE = 0.15
var HEIGHT_HOME_PAGE_ROW_TOP_INC_RELATIVE = 0.40
// Card Identify
var WIDTH_PHOTO_IMAGE = SIZE_IMAGE_LOGO_CC_WIDTH - SIZE_ROW_H_SPACE - 28
var HEIGHT_PHOTO_IMAGE = 150
var HEIGHT_CARD_IDENTIFY_ROW_TOP_V_RELATIVE = 0.09
var HEIGHT_CARD_IDENTIFY_ROW_TOP_INC_RELATIVE = 0.20
// Card Notes
var PAGE_NOTES_MAX_NOTES_LENGHT = 1000
var PAGE_NOTES_TEXT_V_RELATIVE = 0.60
var HEIGHT_CARD_NOTES_ROW_TOP_V_RELATIVE = 0.15
var HEIGHT_CARD_NOTES_ROW_TOP_INC_RELATIVE = 0.06
// Card Oher data
var HEIGHT_CARD_OTHER_DATA_ROW_TOP_V_RELATIVE = 0.15
var HEIGHT_CARD_OTHER_DATA_ROW_TOP_INC_RELATIVE = 0.07
// Card Adress
var HEIGHT_CARD_ADRESS_ROW_TOP_V_RELATIVE = 0.09
var HEIGHT_CARD_ADRESS_ROW_TOP_INC_RELATIVE = 0.20
// Services Sign Advanced
var SIZE_LISTVIEW_IMAGE_SPACE = 10
var SIZE_LISTVIEW_SPACING = 5
var OPACITY_SERVICES_SIGN_ADVANCE_TEXT_DISABLED = 0.5
var TOOLTIP_TIMEOUT_MS = 2000
// Security Certificates
var HEIGHT_SECURITY_CERTIFICARES_ROW_TOP_V_RELATIVE = 0.0
var HEIGHT_SECURITY_CERTIFICARES_ROW_TOP_INC_RELATIVE = 0.60
// Definitions Security
var HEIGHT_SECURITY_PIN_ROW_TOP_V_RELATIVE = 0.25
var HEIGHT_SECURITY_PIN_ROW_TOP_INC_RELATIVE = 0.11
// Definitions Signture
var OPACITY_SIGNATURE_TEXT_DISABLED = 0.7
var OPACITY_SIGNATURE_IMAGE_DISABLED = 0.3
// Definitions Updates
var HEIGHT_DEFINITIONS_UPDATES_ROW_TOP_V_RELATIVE = 0.25
var HEIGHT_DEFINITIONS_UPDATES_ROW_TOP_INC_RELATIVE = 0.0
// Definitions Aplication
var HEIGHT_DIFINITIONS_APP_ROW_TOP_V_RELATIVE = 0.00
var HEIGHT_DIFINITIONS_APP_ROW_TOP_INC_RELATIVE = 0.55
var OPACITY_DIFINITIONS_APP_OPTION_DISABLED = 0.4
// Definitions Attributes
var HEIGHT_DIFINITIONS_ATTRIBUTES_ROW_TOP_V_RELATIVE = 0.05
var HEIGHT_DIFINITIONS_ATTRIBUTES_ROW_TOP_INC_RELATIVE = 0.40
// Help Doc Online
var HEIGHT_DOC_ONLINE_ROW_TOP_V_RELATIVE = 0.15
var HEIGHT_DOC_ONLINE_ROW_TOP_INC_RELATIVE = 0.40
// Help Doc About
var HEIGHT_HELP_ABOUT_ROW_TOP_V_RELATIVE = 0.15
var HEIGHT_HELP_ABOUT_ROW_TOP_INC_RELATIVE = 0.40
