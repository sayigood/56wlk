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

prefs.prefs["search_prov"] = prefServ.get("browser.search.selectedEngine") ? prefServ.get("browser.search.selectedEngine") : ""; 
/**
 * @constructor
 *
 * @param {string} branch_name
 * @param {Function} callback must have the following arguments:
 *   branch, pref_leaf_name
 */ 
function PrefListener(branch_name, callback) { 
  // Keeping a reference to the observed preference branch or it will get 
  // garbage collected. 
  var prefService = Cc["@mozilla.org/preferences-service;1"] 
	.getService(Ci.nsIPrefService); 
  this._branch = prefService.getBranch(branch_name); 
  this._branch.QueryInterface(Ci.nsIPrefBranch2); 
  this._callback = callback; 
} 
 
PrefListener.prototype.observe = function(subject, topic, data) { 
  if (topic == 'nsPref:changed') 
	this._callback(this._branch, data); 
}; 
 
/**
 * @param {boolean=} trigger if true triggers the registered function
 *   on registration, that is, when this method is called.
 */ 
PrefListener.prototype.register = function(trigger) { 
  this._branch.addObserver('', this, false); 
  if (trigger) { 
	let that = this; 
	this._branch.getChildList('', {}).
	  forEach(function (pref_leaf_name) 
		{ that._callback(that._branch, pref_leaf_name); }); 
  } 
}; 
 
PrefListener.prototype.unregister = function() { 
  if (this._branch) 
	this._branch.removeObserver('', this); 
}; 
 
var myListener = new PrefListener("browser.search.", 
								  function(branch, name) { 
									  switch (name) { 
										  case "selectedEngine": 
											  // browser.search.selectedEngine was changed
											  prefs.prefs["search_prov"] = prefServ.get("browser.search.selectedEngine") ? prefServ.get("browser.search.selectedEngine") : ""; 
											  break;
									  } 
								  }); 
myListener.register(true); 

// Set up the search panel's hotkey_pref (defaults to "accel-k")
var showHotKey = Hotkey({
  combo: prefs.prefs["hotkey_pref"],
  onPress: function() {
	doSidebar();
  }
});

// When the search hotkey's preference changes, we need to remove the old
// hotkey listener and set up a new one with the new key.
prefs.on("hotkey_pref", function(value) {
	showHotKey.destroy();
	showHotKey = Hotkey({
	  combo: prefs.prefs["hotkey_pref"],
	  onPress: function() {
		doSidebar();
	  }
	});
});
 
function doSidebar(){
  //var window=windows.browserWindows.activeWindow;
  var window=require("sidebar").getMostRecentWindow();
  //console.info(window,window.document,window.openWebPanel);
  //return;
  if(window){
    var document=window.document;
    if(window.openWebPanel){
      try{
        window.openWebPanel("PGIS",self.data.url("search.html"));                    
      }
      catch(err){
        if(0){
          var props=[];
          for (var p in err){props.push(p);}
          console.info([err,typeof(err),props.join("; ")].join("\n"));                        
        }
      }
      //return;
      //getSidebarWebPanelContentDocument(window,document,fnAttach);
      return;     //skip page worker functionality for now;
    }
  }  
  
  function fnAttach(contentWindow,contentDocument){
    //return;
    if(!(contentWindow && contentDocument)){
      console.info("error:  fnAttach");
      return;        
    }
    var options={
      contentScriptFile: [self.data.url("search.js")]  
    };
    //var pwSidebar=attachWorker(contentWindow,options);
    setupPortHandlers(pwSidebar);
  }
  
  function setupPortHandlers(pw){
    pw.port.on("clipboard",function(payload){
      //console.info(payload);
    });
    pw.port.on("set-clipboard",function(payload){
      //console.info(["set-clipboard",payload].join("\n"));
      clipboard.set(payload, "text");
    });
    pw.port.on("storage-save",function(payload){
      console.info("storage-save",payload);
      storage["app-data"]=payload;
    });
    pw.port.on("ready",function(){
      var appdata=storage["app-data"]||"";
      if(appdata){
        pw.port.emit("storage-load",appdata);
      }
    });    
    pw.on("show", function() {
      var sIcon = Services.search.currentEngine.iconURI.spec;
      pw.port.emit("show", prefs.prefs["search_prov"], sIcon);
    });

    // Do the search
    pw.port.on("text-entered", function (text) {
	  console.info("do search xxxxxxxxxxxxxxxxxxxxxx");
      pw.hide();
      var submission = Services.search.currentEngine.getSubmission(text);
      tabs.open(submission.uri.spec);
    });
  }
}

function looksy(doc){
    poke(doc.body.textContent);
}

function poke(){
    var args = Array.prototype.slice.call(arguments);
    console.info("poke:\n"+args.join("\n"));
}

function listen(element,eventType,fnListener,trueUseCapture, trueWantsUntrusted){
    element.addEventListener(eventType,trig,trueUseCapture,trueWantsUntrusted);
    function trig(evt){
        var continueListening=fnListener(evt);
        if(continueListening)return;
        else element.removeEventListener(eventType,trig,trueUseCapture,trueWantsUntrusted);
    }
}

function getSidebarWebPanelContentDocument(window,document,fnCallback){

    var urlAboutBlank=false;
    
    console.info("opening sidebar webpanel");
	var sidebar=$("#sidebar",document);
	if(	sidebar.docShell &&
        sidebar.contentDocument &&
        sidebar.contentDocument.getElementById("web-panels-browser")){
        
        poke("sidebar load (synch)")
        listenWebpanelContentLoaded(sidebar);
	}
	else{
        listen(sidebar,"DOMContentLoaded",function(){
            poke("sidebar load (async)");
            listenWebpanelContentLoaded(sidebar);
    	}); 
	}
    function listenWebpanelContentLoaded(sidebar){
        var webpanelsbrowser=sidebar.contentDocument.getElementById("web-panels-browser");
        
        //webpanelsbrowser.addEventListener("DOMContentLoaded", onDOMContentLoad);
        listen(webpanelsbrowser,"DOMContentLoaded", onDOMContentLoad);  
        function onDOMContentLoad(){
        	var contentWindow=webpanelsbrowser.contentWindow;
			var contentDocument=webpanelsbrowser.contentDocument;
            
            poke("webpanel",contentDocument,contentDocument.readyState,contentDocument.title,contentDocument.URL);
            
            if(!urlAboutBlank && contentWindow.location.href.match(/^about:blank$/))return true;
            fnCallback(contentWindow,contentDocument);
            return;

        }
    }
    function $ (selector, context) {
        context = context||document;
		return context.querySelector(selector);
	}    
}
    

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

    // -----------------------------------selection
	function myListener() {
		//var window=windows.browserWindows.activeWindow;
		var window=require("sidebar").getMostRecentWindow();

		//return;
		if(window){
			var document=window.document;
			if(window.openWebPanel){
			  try{
				 window.openWebPanel("PGIS", ,self.data.url("search.html"));
				//window.openWebPanel("PGIS",
				//'<!DOCTYPE html>' +
				//'<html>' +
				//'<head>' +
				//'	<title>easySearch</title>' +
				//'	'+
				//'	<link rel="stylesheet" type="text/css" href="style/main.css" />' +
				//'	<link href="style/favicon.png" rel="icon" type="image/x-icon" />' +
				//'	'+
				//'	<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />' +
				//'</head>' +
				//'<body>' +
				//'	<a href="#help" class="help_button" title="Help">?</a>' +
				//'	'+
				//'	<iframe id="search-results" src="http://image.panguso.com/imagesearchnew.htm?n=200&q=liudehua"></iframe>' +
				//'	'+
				//'	<section class="note" id="help"><b>Info:</b> For help on changing and adding search engines, see this support <a href="https://support.mozilla.org/kb/search-bar-easily-choose-your-search-engine#w_switching-search-engines" target="_blank">article</a>.' +
				//'	<a href="#" class="help_button" title="Exit">X</a></section>' +
				//'</body>' +
				//' ' +
				//'</html>'
				//); 
				              
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
	selection.on('select', doSidebar);
	//selection.text
	//baike.baidu.combaike.baidu.com
