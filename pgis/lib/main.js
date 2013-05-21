// This addon is by Joshua Smith
// Sidebar code copied from (jc) Quick Sidebar Unicode Symbol Lookup.  This code is by jongo45

var notifications = require("notifications");
var widget=require("widget");
var clipboard = require("clipboard");
var self=require("self");
var timers=require("timers");
var data = require("self").data;
var prefs = require("simple-prefs");
var prefServ = require("preferences-service");
var { Hotkey } = require("hotkeys");
var {Cu, Cc, Ci} = require("chrome");
var {Services} = Cu.import("resource://gre/modules/Services.jsm");
var tabs = require("tabs");
var windows=require("windows");

// Set up the search panel's hotkey_pref (defaults to "accel-k")
var showHotKey = Hotkey({
  combo: prefs.prefs["hotkey_pref"],
  onPress: function() {
	mySelectListener();
  }
});

// When the search hotkey's preference changes, we need to remove the old
// hotkey listener and set up a new one with the new key.
prefs.on("hotkey_pref", function(value) {
	showHotKey.destroy();
	showHotKey = Hotkey({
	  combo: prefs.prefs["hotkey_pref"],
	  onPress: function() {
		mySelectListener();
	  }
	});
});
 
function mySelectListener(){
  //var window=windows.browserWindows.activeWindow;
  var window=require("sidebar").getMostRecentWindow();
  //console.info(window,window.document,window.openWebPanel);
  //return;
  if(window){
    //var document=window.document;
    if(window.openWebPanel){
      try{
		if(selection.text == null){
			window.openWebPanel("PGIS","http://m.panguso.com/search/image?from=active&adv=0&us=0&q=美女");
		}else{
			window.openWebPanel("PGIS","http://m.panguso.com/search/image?from=active&adv=0&us=0&q="+selection.text);
		}
      }
      catch(err){
        if(0){
          var props=[];
          for (var p in err){props.push(p);}
          console.info([err,typeof(err),props.join("; ")].join("\n"));                        
        }
      }
    }
  }
  return;
}

var selection = require("sdk/selection");
//selection.on('select', myListener);
selection.on('select', mySelectListener);
//selection.text

// This is an active module of the quick anki Add-on
exports.main = function() {
    // ----------------------------Quick Anki
    var panel = require("panel").Panel({
      width: 512,
      height: 1014,
      contentURL: "http://m.panguso.com/search/image?from=active&adv=0&us=0&q=%E5%88%98%E5%BE%B7%E5%8D%8E"
    });
    
    require("widget").Widget({
        id: "mainIcon",
        label: "Quick Image Search On Pangu",
        contentURL: require("self").data.url("style/search.png"),
        panel:panel
    });
    
    // ------------------context-menu
    var popup = function (keywordInj) {
        var panel = require("panel").Panel({
            width: 512,
            height: 1024,
            contentURL: "http://m.panguso.com/search/image?from=active&adv=0&us=0&q=" + keywordInj
        });
        panel.show();
    } 
    
    var cm = require("context-menu");
    cm.Item({
      label: "Search Farnell element14",
      context: cm.SelectionContext(),
      image: 'http://uk.farnell.com/images/farnell_icon.ico',
      contentScript: 'self.on("context", function () {' +
                     '  var text = window.getSelection().toString();' +
                     '  if (text.length > 20)' +
                     '  text = text.substr(0, 20) + "...";' +
                     '  return "Search PanguImage for " + text;' +
                     '});' +
                     'self.on("click", function (node, data) {' +
                     '  var text = window.getSelection().toString();' +
                     '  self.postMessage(text);' +
                     '});',
      onMessage: function (selectionText) {
        popup(selectionText);
      }
    });
    
    var cml = require("context-menu");
    cml.Menu({
      label: "My Menu",
      contentScript: 'self.on("click", function (node, data) {' +
                     '  console.log("You clicked " + data);' +
                     '});',
      items: [
        cm.Item({ label: "Item 1", data: "item1" }),
        cm.Item({ label: "Item 2", data: "item2" }),
        cm.Item({ label: "Item 3", data: "item3" })
      ]
    });

    // -----------------------------greader-panel
    // create the toolbar buton
	var tbb = require("toolbarbutton").ToolbarButton({
	id: "search-panel",
	label: "Quick Image Search On Pangu",
	image: "style/search.png",
	panel: require("panel").Panel({
	  width: 512,
	  height: 1024,
	  contentURL: "http://m.panguso.com/search/image?from=active&adv=0&us=0&q=%E5%88%98%E5%BE%B7%E5%8D%8E"
	})
	});
	
	// move the toolbar button to the addon-bar
	if ("install" == options.loadReason) {
	tbb.moveTo({toolbarID: "nav-bar"});
	}
	
};
