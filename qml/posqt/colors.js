.pragma library

var neon_blue               = "#24d3dc";
var neon_green              = "#8bc53f";
var background              = "#2c2a29";
var transparent_background  = "#a62c2a29";
var transparent_white       = "#11ffffff";
var tran_neon_blue          = "#cc24d3dc";
var tran_white              = transparent_white;//"#ccffffff";
var gray    = "#333333";
var white   = "#ffffff";
var marine  = "#7AC0BD";
var marine_violet = "#7C8BC3"

var gray_two = "#292929";
var gray_three = "#D7D7D2";
var paper   = "#E7DDC4";
var halftran_white          = "#99ffffff";
var toolbar                 = "#353c43";
var tran_toolbar            = "#dd353c43";
var main_blue               = "#299DD6";
var main_green              = "#8bc53f";
var main_yellow             = "#ffe90a";
var calm_red                = "#a53231";
var cidx = 0;
//Grey
var grey2 = "#292929";
var grey3 = "#D7D7D2";
var grey4 = "#A3A093"
var grey5 = "#676767";

//Brown
var     brown   = "#664D48"
//Red
var     red2    = "#99252B"
//Violet
var     violet  = "#827EB0";
var     violet2 = "#918EBA";
var     violet3 = "#3A2B92";
//Pink
var     pink    = "#EA4C89";
var     pink2   = "#C4706E";
var     pink3   = "#E97A7A";

//Blue
var     blue_two  = "#466a82";
var     blue2   = "#466a82";
var     blue3   = "#089ADB";
var     blue4   = "#73C3F4";
var     blue5   = "#282298"
var     blue6   = "#014F6E"; //!< #014F6E
var     blue7   = "#4DFFFF";
//Green
var     green2  = "#557134";
var     green3  = "#55A235";
var     green4  = "#A4A534";
var     green5  = "#172F17";
//Orange
var     orange  = "#ff6600"
var     orange2 = "#E4882E"
var     orange3 = "#D6513E";

//Yellow
var     yellow2 = "#FED61D";
var     yellow3 = "#F3A61D";
var     yellow4 = "#EFC62E";

var color_pc = 0;

//Color lists, helpfull in auto colorizing components
var colorList = [ main_blue, violet, orange, green2, marine,
                  yellow3, pink2, violet2, blue3, brown,
                  pink, marine_violet, orange2, violet3 ];

var menuColors = [ violet2, green2, blue4, orange2, pink2, brown, blue5 ];

var opc_half                = "99";
var opc_full                = "cc";

function index_color() {
    var clr = colorList[color_pc];
    color_pc++;
    if( color_pc > (colorList.length - 1) ) {
        color_pc = 0;
    }

    return clr;
}

function pc() { var i = color_pc; color_pc = (color_pc + 1 > (colorList.length - 1)) ? 0 : color_pc + 1; return i; }
function index( index ) {
    /*if( index && index > -1 && index < colorList.length )
        return colorList[index]; */
    return colorList[ index % 14 ];
}
function indexMenu( index ) {
    //if( index && index > -1 && index < menuColors.length )
        return menuColors[index % 7];
}

function make( definition, opacity ){
    if( !definition )
        return "#99ff00ff";

    definition = String(definition);

    if( definition.charAt(0) === "#" )
        definition = definition.slice(1);

    if( definition.length === 8 ) {
        definition = definition.slice(2);
    }
    else if( definition.length !== 6 ) {
        return "#99ff00ff";
    }

    if( opacity )
        return "#" + opacity + definition;

    return "#" + definition;
}

function has_opacity( color_string ) {
    color_string = String(color_string);

    if( color_string.charAt(0) === "#" )
        color_string = color_string.slice(1);

    if( color_string.length > 6 ) {
        return true;
    }

    return false;
}

function mulitply( left, right )
{
    return "#9900ffff";
}
