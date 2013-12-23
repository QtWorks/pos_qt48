.pragma library

var api = null;

var _undefined_ = -1;
var last_name;//   = "";
var first_name;//  = "";
var title       = "";
var clockedin   = false;
var level       = _undefined_;
var id          = _undefined_;
var merchant    = _undefined_;
var cashier     = null;
var bank        = _undefined_;
var cashout     = null;
var make_cashier = null;
var start_staff_bank = null;
var seconds_on_clock_today = null;
var clockout = null;

function _init( _api ) {
    api = _api;

    //_api.login( "101" );
    seconds_on_clock_today  = api.seconds_on_clock_today;
    clockout                = api.clockout;
    make_cashier            = api.make_cashier;
    start_staff_bank        = api.start_staff_bank;
}

//Returns user id or _undefined_
function login( pass ) {
    id = api.login( pass );
    if( id != _undefined_ ) {
        last_name   = api.lastName;
        first_name  = api.firstName;
        title       = api.title;
        level       = api.level;
        id          = api.uid;
        clockedin   = api.clockedin;
        merchant    = api.merchant;
        cashier     = api.cashier
        cashout     = api.cashout;
        bank        = api.bank;
    }

    return id;
}

function logout()
{
}

function clockin()
{
    api.clockin();
    clockedin = api.clockedin;
}

/*function clockout()
{
    api.clockout();
    clockedin = api.clockedin;
}*/
