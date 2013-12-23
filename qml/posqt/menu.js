.pragma library

var api;
var catItems = [];
var add_comment = null;
var select      = null;
var hassubs     = null;
var navigate    = null;
var cat_color_map = [];

function map_cat_color( catid )
{
    if( cat_color_map.length ) {
        for(var i = 0; i < cat_color_map.length; i++) {
            if( cat_color_map[i] === catid ) {
                return i;
            }
        }
    }

    var index = cat_color_map.length;
    cat_color_map[index] = catid;
    return index;
}

function bind_category(cat, catid)
{
    catItems[catid] = cat;
}

function init( _api ) {
    api         = _api;
    add_comment = api.add_comment;
    select      = api.select;
    hassubs     = api.hassubs;
    navigate    = api.navigate;
}

function get_itemModel() {
    return api.itemModel;
}

function get_catModel() {
    return api.catModel;
}
