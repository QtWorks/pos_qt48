.pragma library

var api = null;
var set_itemname = null;
var set_itemprice = null;
var new_item = null;
var delete_item = null;
var save_item = null;
var select = null;
var menuitem_modifiable = null;

var name_change_cb = null;

function init( _api_ ) {
    api = _api_;
    set_itemname = api.set_itemname;
    set_itemprice = api.set_itemprice;
    new_item = api.new_item;
    delete_item = api.delete_item;
    save_item = api.save_item;
    select = api.select;
    menuitem_modifiable = api.menuitem_modifiable;
}
