/*
  These functions are used by app.js.
  Are in separate file for readability sake

  NOTE: DO NOT USE THIS FILE EXPLICITLY. DOING SO WILL BREAK FUNCTIONALITY

  does NOT WORK if not included trough 'app.js'
*/


//User login/logout

function switchUser ()
{
    if ( valid() ) {
        _root.switchUser();
        curViewName = "UserView";
    }
}

function checkLogin( pass )
{
    if ( valid() ) {
        return _API.CheckLogin( pass );
    }
    else {
        return -1;
    }
}

function loginUser( user_id )
{
    if ( valid() ) {
        userid = user_id;
        return _API.LoginUser( user_id );
    }
    else {
        return -1;
    }
}

function logoutUser()
{
    if ( valid() ){
        _root.disableNavbar();
        _API.LogOutUser();
        _root.hideNavbar();
        curViewName = "LoginView";
        _root.loadLoginScreen();
    }
}
