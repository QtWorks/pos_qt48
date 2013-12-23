/*
  These functions are used by app.js.
  Are in separate file for readability sake

  NOTE: DO NOT USE THIS FILE EXPLICITLY. DOING SO WILL BREAK FUNCTIONALITY

  does NOT WORK if not included trough 'app.js'
*/

//View navigation
function hide_choice()
{
    _root.hide_choice();
}

function showLoginScreen()
{
    if ( valid() ) {
        curViewName = "LoginView";
        _root.hideNavbar();
        _root.loadLoginScreen();
    }
}

function showTableScreen()
{
    if ( valid() ) {
        curViewName = "TableView";
        _root.loadTableScreen();
        _root.hideNavbar();
    }
}

function showCurrentUserScreen()
{
    if ( valid() ) {
        curViewName = "UserView";
        _root.hideNavbar();
        _root.switchUser();
    }
}

function showSplitTicketScreen()
{
    if ( valid() ) {
        curViewName = "SplitView";
        _root.hideNavbar();
        _root.loadSplitTicketScreen();
    }
}

function showLastScreen() {}

//Navigation bar
function enableNavbar() {
    if ( valid() && _root) {
        _root.enableNavBar();
    }
}

function disableNavbar() {
    if ( valid() && _root) {
        _root.disableNavbar();
    }
}

function hideNavbar() {
    if ( valid() && _root) {
        _root.hideNavbar();
    }
}

function navBack() {
    _root.navBack();
}

//Custom callback on back button pressed
function setBackCB( callback ) {
    backCB = callback;
}
