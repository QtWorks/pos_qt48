.pragma library
Qt.include( "app_nav.js" );
Qt.include( "app_user.js" );

var _API;
var _initialized = false;
var _root;
var admin_console;
var _loader;
var bg_filepath = "rok_black.png";
var table_Model;
var asale_Model;
var curViewName = "undefined";
var tableSaleModel;
var refresh;
var reserve_table;
var table_isreserved;
var tableid = 0;
var get_user_id;

var popupCB = null;
var backCB = null;

//Globar variables
var screen_h        = 900;//1080;
var screen_w        = 1600;//1920;

//User info
var username        = "undefined";
var usertitle       = "undefined";
var userid          = -1;
var userlevel       = -1;

var keyboard_callback = undefined;

//Configs
var UserView = {}
UserView.tableBackdrop = {
    width : 800
}
UserView.tableDelegate = {
    width   : 600,
    height  : 120
}
UserView.tableDelegate.circle = {
    height  : UserView.tableDelegate.height/2,
    width   : UserView.tableDelegate.height/2,
    radius  : 10
}
UserView.tableList = {
    height  : 400,
    width   : 600
}

function getUserId() { return userid; }

//Helpful values
var months = [ "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" ];

function init( api ) {
    if ( !_initialized ) {
        _API = api;
        _initialized = true;
        tableSaleModel  = api.tableSaleModel;
        refresh = api.refresh;
        reserve_table   = api.reserve_table;
        table_isreserved = api.table_isreserved;
        get_user_id          = api.get_user_id;
    }
}

function unreserve_table( table )
{
    reserve_table( table, "" );
}

function valid() {
    return ( _initialized && _API );
}

function exit () {
    if ( valid() ) {
        _root.exitQML();
    }
}

function warning ( message ) {
    if( valid() ) {
        _root.warning( message );
    }
}

function choice ( message, callback ) {
    if( valid() ){
        popupCB = callback;
        _root.choice( message );
    }
}

function get_saleModel () {
    return _API.activeSaleModel;
}

function get_tableSaleModel() {
    return _API.tableSaleModel;
}

function get_tableModel () {
    return _API.tableListModel;
}

function setRoot( root ) {
    if ( valid() ) {
        _root = root;
        //_API.SetRoot( _root );
    }
}

function setLoader( loader ) {
    if ( valid() ) {
        _loader = loader;
    }
}
function addNewSale( table_id ) {
    return _API.addNewSale( table_id );
}


function editTicket() {
    if ( valid() ) {
        curViewName = "MenuView";
        _root.loadMenuScreen();
        _root.hideNavbar();
        //_API.EditTicket();
    }
}

function refreshActiveSales()
{
    _API.refreshActiveSales();
}

function refreshTables()
{
    _API.refreshTables();
}

function activateTable( table_id ) {
    if ( valid() ) {
        tableid = table_id;
        _API.ActivateTable( table_id );
    }
}

function runTicketTest( ) {
    if ( valid() ) {
        loginUser( 2 );
        _loader.setSource( "tickettest.qml" );
    }
}

function show_osk( cb ) {
    _root.show_osk( cb );
}

function hide_osk() {
    _root.hide_osk();
}

function admin_console_set_interactive( interactive ) {
    admin_console.set_interactive( interactive );
}

function load_subview( component ) {
    _root.load_subview( component );
}
