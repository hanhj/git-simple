!function(e){if("function"==typeof define&&window.define.amd)window.define([],e);else e()}(function(){var e="2020-01-02";var t="";window.URSCFG={};window.URSOPENBGP="{URSOPENBGPVALUE}";var i={},n={},o={},r={},a=[],s=0;if(!Function.prototype.bind)Function.prototype.bind=function(e){if("function"!=typeof this)throw new TypeError("Function.prototype.bind - what is trying to be bound is not callable");var t=Array.prototype.slice.call(arguments,1),i=this,n=function(){},o=function(){return i.apply(this instanceof n&&e?this:e,t.concat(Array.prototype.slice.call(arguments)))};if(this.prototype)n.prototype=this.prototype;o.prototype=new n;return o};var c=function(e){return e.replace(/\/\/([^\/]+:?)\//,function(e,t){var i=t;i=i.replace(/([^\.]+:?)\./,function(e,t){return t+"-v6."});return"//"+i+"/"})};var l=function(e){e=parseInt(e,10);return e>=3};var f=function(){var e=navigator.userAgent.toLowerCase();var t=/msie/.test(e)&&!/opera/.test(e);var i=(e.match(/.+(?:rv|it|ra|ie)[\/: ]([\d.]+)/)||[0,"0"])[1];var n={6:"2.0",7:"3.0",8:"4.0",9:"5.0",10:"6.0",11:"7.0"};var o=n[document.documentMode]||n[parseInt(i)];if(t&&parseInt(o,10)<4)return 1;else return 0};var u=function(e,t,i){if(window.addEventListener)e.addEventListener(t,i,!1);else e.attachEvent("on"+t,i)};var h=function(e,t,i){if(window.removeEventListener)e.removeEventListener(t,i);else e.detachEvent("on"+t,i)};var d=function(e){e=e||"";if((e.indexOf("passport.")>=0||e.indexOf("dl.reg.163.com")>=0||e.indexOf("reg.icourse163.org")>=0)&&e.indexOf("/webzj")>=0)e=e.replace(/\:\/\/[^\/]+\/webzj\//,function(e){return e+"b/"});else e=e.replace(/\:\/\/([^\/]+)/,function(e){return e+"/b"});return e};var p=[];var g=function(e){var t="";var i=o[e];if(i.__coverBackground&&I("animation"))t=i.__coverBackground.indexOf("background:")!=-1?i.__coverBackground:"";return"position:fixed;_position:absolute;top:0;left:0;width:100%;height:100%;overflow:hidden;background:rgb(0,0,0); filter:progid:DXImageTransform.Microsoft.Alpha(opacity=60);-moz-opacity:0.6;-khtml-opacity:0.6;opacity:0.6;z-index:1000;"+t};var m=function(e,t){return"position:fixed;_position:absolute;z-index:10000;left:50%;top:50%;width:"+e+"px;margin-left:-"+e/2+"px;height:"+t+"px;margin-top:-"+t/2+"px;"};var v=function(e){var t=o[e];var i=null;if(t.__iframeShowAnimation)i="-webkit-animation:"+t.__iframeShowAnimation+";-moz-animation:"+t.__iframeShowAnimation+";-ms-animation:"+t.__iframeShowAnimation+";-o-animation:"+t.__iframeShowAnimation+";animation:"+t.__iframeShowAnimation+";";return"width:100%;height:100%;border:none;background:none;"+(i?i:"")};var w=function(){var e=setInterval(function(){for(var t=0;t<a.length;t++)if(a[t].readyDone){e=clearInterval(e);a.shift();b(1);break}},200)};var b=function(e){if(e||!s){s=1;var t=setInterval(function(){for(var e=0;e<a.length;e++)if(!a[e].readyDone){t=clearInterval(t);k.call(a[e]);w();break}},200)}};var _=function(e,t,i){var n=i.id;var r="x-URS-iframe"+n;var s=o[n];var c=document.getElementById(r),l=s._name||"";if(!c){try{c=document.createElement('<iframe  name="'+l+'" allowTransparency=true ></iframe>')}catch(f){c=document.createElement("iframe");c.allowTransparency=!0;c.name=l}c.frameBorder=0;c.id=r;c.scrolling="no";c.style.cssText=v(n)}if(t)e.appendChild(c);else{var u=420,h=408;if(s.frameSize){u=s.frameSize.width;h=s.frameSize.height}var d=document.getElementById("x-discover"+n);if(!d){d=document.createElement("div");d.id="x-discover"+n;d.style.cssText=g(n)}var p=document.getElementById("x-panel"+n);if(!p){p=document.createElement("div");p.id="x-panel"+n;s._panel=p;p.style.cssText=m(u,h)}p.appendChild(c);e.appendChild(d);e.appendChild(p);e.style.display="none"}if(!window.postMessage){a.push(this);b(0)}};var y=function(){var e=window.URSCFG[this.MGID];this._url_cache=this._url_cache.replace("webzj.reg.163.com","webzj2.reg.163.com");if(e._$passportNeedUrsBgp)this._url_cache=this._url_cache.replace("passport.","passport2.").replace("reg.icourse163.org","reg2.icourse163.org").replace("dl.reg.163.com","dl2.reg.163.com");if(this._urs_options.wdaId)this._url_cache=this._url_cache.replace(/wdaId=([^&]+)/,"wdaId=UA1482833332087")};var S=function(e){var i="x-URS-iframe"+this.MGID;var n=document.getElementById(i);if(this._urs_options&&this._urs_options.afterSetIframeSrc)this._urs_options.afterSetIframeSrc(n);if("{URSOPENBGPVALUE}"!=window.URSOPENBGP)y.call(this);window.setTimeout(function(){this.__loadTime=(new Date).getTime();if(n){if("is_IPV6_Message"===t)if(e!=-1&&1!=e&&"{URSOPENBGPVALUE}"===window.URSOPENBGP)this._url_cache=c(this._url_cache);n.src=this._url_cache}}.bind(this),0);if(e!==-1){this.sto=clearTimeout(this.sto);var o=window.URSCFG[this.MGID];if(o._$needUrsBgp&&"{URSOPENBGPVALUE}"===window.URSOPENBGP)if(1!=e){this.sto=setTimeout(function(){this.sto=clearTimeout(this.sto);y.call(this);S.call(this,1)}.bind(this),this._urs_options.bgpTime);return}this.sto=setTimeout(function(){this.sto=clearTimeout(this.sto);var e=location.href||"";e=e.substring(0,200);e=encodeURIComponent(e);this._url_cache="//hc.reg.163.com/webcomponent/guide.html?pkid="+this._urs_options.promark+"&pd="+this._urs_options.product+"&time="+(new Date).getTime()+"_"+[0,1,2,3,4,5,6,7,8,9].sort(function(){return Math.random()>.5?1:-1}).join("")+"&from="+e;S.call(this,-1)}.bind(this),this._urs_options.bgpTime)}};var I=function(e){var t=["webkit","Moz","ms","o"],i,n=[],o=document.documentElement.style,r=function(e){return e.replace(/-(\w)/g,function(e,t){return t.toUpperCase()})};for(i in t)n.push(r(t[i]+"-"+e));n.push(r(e));for(i in n)if(n[i]in o)return!0;return!1};var M=function(e,i){var n=document.getElementById("x-URS-iframe"+e);var o=window.name||"_parent";var r={};r.data=i;r.data.from="URS|";r.data.topURL=location.href||"";r.origin="*";r.source=o;if("is_IPV6_Message"===t)r.data.mv="new_cdn_101_v6";else r.data.mv="new_cdn_101";r.data.loadTime=(new Date).getTime()-this.__loadTime;if(n)D(n.contentWindow,r)};var k=function(){M.call(this,this.MGID,this._urs_options)};var U=function(){var e=/^([\w]+?:\/\/.*?(?=\/|$))/i;return function(t){t=t||"";if(e.test(t))return RegExp.$1;else return"*"}}();var C=function(e,t){try{t=t.toLowerCase();if(null===e)return"null"==t;if(void 0===e)return"undefined"==t;else return Object.prototype.toString.call(e).toLowerCase()=="[object "+t+"]"}catch(i){return!1}};var R=function(e,t,i){if(!e)return"";var n=[];for(var o in e)if(e.hasOwnProperty(o)){var r=e[o];if(r)if(!C(r,"function")){if(C(r,"date"))r=r.getTime();else if(C(r,"array"))r=r.join(",");else if(C(r,"object"))r=JSON.stringify(r);if(i)r=encodeURIComponent(r);n.push(encodeURIComponent(o)+"="+r)}else;else;}else;return n.join(t||",")};var D=function(){var e="MSG|";var t=function(t){var i={};t=t||{};i.origin=t.origin||"";i.ref=location.href;i.self=t.source;i.data=JSON.stringify(t.data);return e+R(i,"|",!0)};return function(e,i){if(window.postMessage){i=i||{};e.postMessage(JSON.stringify(i.data),U(i.origin))}else p.unshift({w:e,d:escape(t(i))})}}();var x=function(){var e=navigator.appName;if("Netscape"==e){var t=window.open("about:blank","_self");t.opener=null;t.close()}else if("Microsoft Internet Explorer"==e){window.opener=null;window.open("","_self");window.close()}};var T=function(e,i){var n;var o=0!=e.isHttps?"https://":"http://";if(e.cssDomain&&e.cssFiles)if(e.cssDomain.indexOf("http://")!=-1)o="http://";if(l(e.version)){n="index2.html";if(e.single){n="index_dl2.html";if("register"==e.page)n="index_reg2.html"}}else{n="index.html";if(e.single){n="index_dl.html";if("register"==e.page)n="index_reg.html"}}if("1"==e.newCDN)n=n.replace(".html","_new.html");var r;if("is_IPV6_Message"===t)r=e.crossDomainUrl||"webzj.reg.163.com/v6/pub/";else r=e.crossDomainUrl||"webzj.reg.163.com/v1.0.1/pub/";this._url_cache=o+r+n;var a=parseInt(1e3*Math.random());e.pathB=0;var s=window.URSCFG[e.promark];if(s)if(a<=s)e.pathB=1;if(e.pathB)this._url_cache=d(this._url_cache);if(i.__cssStr)this._url_cache+="?"+i.__cssStr+"&MGID="+this.MGID+"&wdaId="+(e.wdaId||"");else this._url_cache+="?MGID="+this.MGID+"&wdaId="+(e.wdaId||"");this._urs_options=e||{};this._urs_options.bgpTime=e.bgpTime||1e4;this._url_cache+="&pkid="+(this._urs_options.promark||"")+"&product="+(this._urs_options.product||"");if(e.cdnhostname)this._url_cache+="&cdnhostname="+e.cdnhostname;return this._url_cache};window.URS=function(){var e=function(){var t=(new Date).getTime()+Math.random();if(!r[t]){r[t]=t;return t}else return e()};var a=function(e){var i=e.cookieDomain||"";var n=e.regCookieDomain||"";var o=e.crossDomainUrl||"";var r,a;if(!o){if(i){e.regCookieDomain=i;r=i}else if(n){e.cookieDomain=n;r=n}if("is_IPV6_Message"===t)a="v6";else a="v1.0.1";if(r)if(r.indexOf("icourse163")>-1)e.crossDomainUrl="reg."+r+"/webzj/"+a+"/pub/";else e.crossDomainUrl="passport."+r+"/webzj/"+a+"/pub/";else e.crossDomainUrl="dl.reg.163.com/webzj/"+a+"/pub/"}return e};var s=function(t){this.MGID=e();window.URSCFG[this.MGID]={};var i=window.URSCFG[this.MGID];t.from3Cdn=1;if(t.from3Cdn&&l(t.version))t=a.call(this,t);if(f())t.needUrsBgp=0;if("0"==t.needUrsBgp){t.passportNeedUrsBgp=0;i._$needUrsBgp=0;i._$passportNeedUrsBgp=0}else{if(t.crossDomainUrl||t.cookieDomain){i._$passportNeedUrsBgp=1;t.passportNeedUrsBgp=1}i._$needUrsBgp=1;t.needUrsBgp=1}o[this.MGID]={};this._$COM_NUM=1==this._$COM_NUM?1:2;var n=o[this.MGID];n.promark=t.promark;n.frameSize=t.frameSize;n.__coverBackground=t.coverBackground;n.__iframeShowAnimation=t.iframeShowAnimation;if(t.cssDomain&&t.cssFiles)n.__cssStr="cd="+encodeURIComponent(t.cssDomain)+"&cf="+encodeURIComponent(t.cssFiles);this.isInclude=0;if(t.includeBox)if("string"==typeof t.includeBox)this.isInclude=document.getElementById(t.includeBox)||0;else this.isInclude=t.includeBox;n.needPrepare=t.needPrepare||0;if("string"==typeof t.eventType)this._type=t.eventType;if("string"==typeof t.bid)this._btn=document.getElementById(t.bid);else this._btn=t.bid;if(t.logincb)this.logincb=t.logincb;if(t.closecb)this.closecb=t.closecb;if(t.regcb)this.regcb=t.regcb;if(t.loginCheckLock)this.loginCheckLock=t.loginCheckLock;
if(t.regCheckLock)this.regCheckLock=t.regCheckLock;if(t.initReady)this.initReady=t.initReady;if(t.statecb)this.statecb=t.statecb;if(t.resize)this.resize=t.resize;if(t.changepage)this.changepage=t.changepage;if(t.moduleResize)this.moduleResize=t.moduleResize;if(t.loginstate)this.loginstate=t.loginstate;if(t.otherRegSuccess)this.otherRegSuccess=t.otherRegSuccess;if(t.lockMbLoginState)this.lockMbLoginState=t.lockMbLoginState;if(t.lockMbRegState)this.lockMbRegState=t.lockMbRegState;if(t.mbInitSuccess)this.mbInitSuccess=t.mbInitSuccess;if(t.mbChangeModule)this.mbChangeModule=t.mbChangeModule;if(t.loginInitSuccess)this.loginInitSuccess=t.loginInitSuccess;if(t.regInitSuccess)this.regInitSuccess=t.regInitSuccess;if(t.renderOk)this.renderOk=t.renderOk;if(t.WeiXinInputBlur)this.WeiXinInputBlur=t.WeiXinInputBlur;if(t.loginEmailValue)this.loginEmailValue=t.loginEmailValue;if(t.loginMbValue)this.loginMbValue=t.loginMbValue;var r=document.createElement("div");r.id="x-URS"+this.MGID;document.body.appendChild(r);this.box=r;this._url_cache=T.call(this,t,n);try{JSON.stringify(this._urs_options)}catch(s){return null}if(!this.isInclude){if(this._btn&&this._type)u(this._btn,this._type,this.showIframe.bind(this))}else this.includeBox=this.isInclude};var c=function(e){if(e)e.stopPropagation?e.stopPropagation():e.cancelBubble=!0};var d=function(e){c(e);var t=e.data||"{}";if("string"==typeof t)try{t=JSON.parse(t)}catch(n){t={}}if(i[t.MGID])i[t.MGID]({data:t,origin:U(e.origin)})};var g=function(e){var t=e.data,i,r,a;if(t){if("string"==typeof t)try{t=JSON.parse(t)}catch(s){t={}}if(t.MGID){i=n[t.MGID];r=o[t.MGID];if(i.isInclude)a=i.includeBox;else a=r._panel;if(t["URS-READY-DONE"]){i.readyDone=1;i.sto=clearTimeout(i.sto);if(i.initReady)i.initReady()}if(t["URS-READY"]){i.sto=clearTimeout(i.sto);i.ursReady=1}if(!window.postMessage||!t["URS-READY"]||!i.isInclude&&r.needPrepare){if(t["URS-READY"]&&!r._initReady)r._initReady=!0;if(!t["URS-CM-STATE"])if(!t||!t.fromOutLogin||t.toOpener){if("loginEmailValue"===t.type){if(i.loginEmailValue)i.loginEmailValue(t)}else if("loginMbValue"===t.type){if(i.loginMbValue)i.loginMbValue(t)}else if("WeiXinInputBlur"===t.type){if(i.WeiXinInputBlur)i.WeiXinInputBlur()}else if("renderOk"==t.type){if(i.renderOk)i.renderOk(t)}else if("moduleResize"==t.type){if(i.moduleResize)i.moduleResize(t)}else if("regInitSuccess"==t.type){if(i.regInitSuccess)i.regInitSuccess()}else if("loginInitSuccess"==t.type){if(i.loginInitSuccess)i.loginInitSuccess()}else if("mbChangeModule"==t.type){if(i.mbChangeModule)i.mbChangeModule()}else if("mbInitSuccess"==t.type){if(i.mbInitSuccess)i.mbInitSuccess()}else if("lockMbLoginState"==t.type){if(i.lockMbLoginState)i.lockMbLoginState(t)}else if("lockMbRegState"==t.type){if(i.lockMbRegState)i.lockMbRegState(t)}else if("otherRegSuccess"==t.type){if(i.otherRegSuccess)i.otherRegSuccess(t)}else if("success"==t.type){if(i.logincb)i.logincb(t["username"],t["isOther"],t);if(!this.isInclude){if(i._btn&&i._type)h(i._btn,i._type,i.showIframe.bind(i));i.closeIframe()}}else if("close"==t.type){if(i.closecb)i.closecb();i.closeIframe()}else if("resize"==t.type||"init"==t.type){a.style.width=t.width+"px";a.style.height=t.height+"px";if(!i.isInclude)a.style.marginLeft=-1*t.width/2+"px";if(i.resize)i.resize(t)}else if("register-success"==t.type){if(i.regcb)i.regcb(t["username"],t["url"])}else if("lockLoginState"==t.type){if(i.loginCheckLock)i.loginCheckLock(t.value)}else if("lockRegState"==t.type){if(i.regCheckLock)i.regCheckLock(t.value)}else if("changepage"==t.type){if(i.changepage)i.changepage(t.page)}else if("loginstate"==t.type)if(i.loginstate)i.loginstate(t)}else{try{window.opener.$outLogin(t)}catch(s){}setTimeout(function(){x()},200)}else if(i.statecb)i.statecb(t["URS-CM-STATENAME"],t["URS-CM-STATE"])}else k.call(i)}}};var m=function(){var e="MSG|";var t=function(e,t){var i=C(t,"function")?t:function(e){return e===t},n=null;for(var o=0,r=e.length-1,a;o<r;o++){a=e[o];if(i(a))n=o}return null!=n?n:-1};var i=function(){var e;var i=function(i,n,o){if(t(e,i.w)<0){e.push(i.w);o.splice(n,1);i.w.name=i.d}};return function(){e=[];if(p&&p.length)for(var t=p.length,n;t--;t>=0){n=p[t];i(n,t,p)}e=null}}();var n=function(){var t=unescape(window.name||"");if(t&&0==t.indexOf(e)){window.name="";var i=t.replace(e,""),n=i.split("|"),o=n.length,r={};for(var a=0;a<o;a++){var s=n[a].split("=");if(!s||!s.length)return;var c=s.shift();if(!c)return;r[decodeURIComponent(c)]=decodeURIComponent(s.join("="))}i=r;var l=(i.origin||"").toLowerCase();if(!l||"*"==l||0==location.href.toLowerCase().indexOf(l))g({data:i.data||"null",origin:U(i.ref||document.referrer)})}};return function(){setInterval(i,100);setInterval(n,20)}}();var v=function(){if(!window.__hasRun){if(window.postMessage)u(window,"message",d);else m();window.__hasRun=1}};return function(e){s.call(this,e);var t=o[this.MGID];if(t.needPrepare||this.isInclude)this.prepareIframe();i[this.MGID]=g.bind(this);n[this.MGID]=this;return v()}}();window.URS.prototype.prepareIframe=function(){if(this.isInclude){_.call(this,this.includeBox,1,{id:this.MGID});S.call(this);this.showIframe()}else{_.call(this,this.box,0,{id:this.MGID});S.call(this)}};window.URS.prototype.showIframe=function(e){var t=o[this.MGID];if(!this.isInclude)if(!t.needPrepare){_.call(this,this.box,0,{id:this.MGID});S.call(this)}else if(!t._initReady)return;e=e||{};if(e.page){if(100*Math.random()<=1)try{var i="//webzj-v6.reg.163.com/UA1435545636633/__utm.gif?log=usepage&pd="+this._urs_options.product||"";var n=document.createElement("img");n.style.position="absolute";n.style.width="0px";n.style.height="0px";document.body.appendChild(n);n.src=i;setTimeout(function(){document.body.appendChild(n)},1e4)}catch(r){}if(e.page!=this._urs_options.page&&this._urs_options.single){this._urs_options.page=e.page;this._url_cache=T.call(this,this._urs_options,t)}S.call(this)}if(t.needPrepare&&!this.isInclude)k.call(this);this.box.style.display="block";if(this._urs_options.afterShow)this._urs_options.afterShow.call(this)};window.URS.prototype.closeIframe=function(){var e=o[this.MGID];if(!this.isInclude){this.box.style.display="none";if(this.sto)this.sto=clearTimeout(this.sto);if(!e.needPrepare){if(navigator.userAgent.indexOf("MSIE")>0){var t=document.getElementById("x-URS-iframe"+this.MGID),i=t.contentWindow;if(t){t.src="about:blank";try{i.document.write("");i.document.clear()}catch(n){}}var r=document.getElementById("x-panel"+this.MGID);r.removeChild(t);window.CollectGarbage()}this.box.innerHTML=""}}else;};window.URS.prototype.loginUnlock=function(){var e={fromLoginLock:1,lock:0};M.call(this,this.MGID,e)};window.URS.prototype.loginDolock=function(){var e={fromLoginLock:1,lock:1};M.call(this,this.MGID,e)};window.URS.prototype.regUnlock=function(){var e={fromRegLock:1,lock:0};M.call(this,this.MGID,e)};window.URS.prototype.regDolock=function(){var e={fromRegLock:1,lock:1};M.call(this,this.MGID,e)};window.URS.prototype.doLoginProxy=function(e){var t={username:e.username,pwd:e.pwd,defaultUnLogin:e.defaultUnLogin,doLoginProxy:1};M.call(this,this.MGID,t)};window.URS.prototype.loginUnlockMb=function(){var e={fromLoginLockMb:1,lock:0};M.call(this,this.MGID,e)};window.URS.prototype.loginDolockMb=function(){var e={fromLoginLockMb:1,lock:1};M.call(this,this.MGID,e)};window.URS.prototype.regUnlockMb=function(){var e={fromRegLockMb:1,lock:0};M.call(this,this.MGID,e)};window.URS.prototype.regDolockMb=function(){var e={fromRegLockMb:1,lock:1};M.call(this,this.MGID,e)};window.URS.prototype.getIframeSize=function(){var e={getIframeSize:1};M.call(this,this.MGID,e)};window.URS.prototype.setMbloginClause=function(e){var t={fromMbSetClause:1,mbloginClause:e};M.call(this,this.MGID,t)};window.URS.prototype.setMailloginClause=function(e){var t={fromMailSetClause:1,mailloginClause:e};M.call(this,this.MGID,t)};window.URS.setPkidAndPd=function(){var e={};var i=function(t){var i,n;if(t&&t.lgs){i=t.lgs;n=t.pkid;window.URSCFG[n]=parseInt(i);e[n]&&e[n](n)}};var n=function(e){var n=e.pkid||"";var o=e.pd||"";var r;if(void 0===e.mode)r="3";else r=e.mode;if("3"!=r&&"0"!=r){var a="URSJSONP"+(new Date).getTime();window[a]=i;var s="//dl.reg.163.com/dl/getConf?callback="+a+"&pkid="+n+"&pd="+o+"&mode="+r;var l=document.createElement("script");l.type="text/javascript";l.id="urs-script-"+a;if("is_IPV6_Message"===t)s=c(s);l.src=s;document.getElementsByTagName("head")[0].appendChild(l);setTimeout(function(){document.getElementsByTagName("head")[0].removeChild(l)},5e3)}};return function(t){t=t||{};var i=t.pkid||"";if("function"==typeof t.pathbCallback)e[i]=t.pathbCallback;n(t)}}();return window.URS});(function(){function e(t,n){function r(e){if(r[e]!==v)return r[e];var t;if("bug-string-char-index"==e)t="a"!="a"[0];else if("json"==e)t=r("json-stringify")&&r("json-parse");else{var i;if("json-stringify"==e){t=n.stringify;var o="function"==typeof t&&w;if(o){(i=function(){return 1}).toJSON=i;try{o="0"===t(0)&&"0"===t(new a)&&'""'==t(new s)&&t(p)===v&&t(v)===v&&t()===v&&"1"===t(i)&&"[1]"==t([i])&&"[null]"==t([v])&&"null"==t(null)&&"[null,null,null]"==t([v,p,null])&&'{"a":[1,true,false,null,"\\u0000\\b\\n\\f\\r\\t"]}'==t({a:[i,!0,!1,null,"\0\b\n\f\r\t"]})&&"1"===t(null,i)&&"[\n 1,\n 2\n]"==t([1,2],null,1)&&'"-271821-04-20T00:00:00.000Z"'==t(new l((-864e13)))&&'"+275760-09-13T00:00:00.000Z"'==t(new l(864e13))&&'"-000001-01-01T00:00:00.000Z"'==t(new l((-621987552e5)))&&'"1969-12-31T23:59:59.999Z"'==t(new l((-1)))}catch(c){o=!1}}t=o}if("json-parse"==e){t=n.parse;if("function"==typeof t)try{if(0===t("0")&&!t(!1)){i=t('{"a":[1,true,false,null,"\\u0000\\b\\n\\f\\r\\t"]}');var f=5==i.a.length&&1===i.a[0];if(f){try{f=!t('"\t"')}catch(u){}if(f)try{f=1!==t("01")}catch(h){}if(f)try{f=1!==t("1.")}catch(d){}}}}catch(g){f=!1}t=f}}return r[e]=!!t}t||(t=o.Object());n||(n=o.Object());var a=t.Number||o.Number,s=t.String||o.String,c=t.Object||o.Object,l=t.Date||o.Date,f=t.SyntaxError||o.SyntaxError,u=t.TypeError||o.TypeError,h=t.Math||o.Math,d=t.JSON||o.JSON;"object"==typeof d&&d&&(n.stringify=d.stringify,
n.parse=d.parse);var c=c.prototype,p=c.toString,g,m,v,w=new l((-0xc782b5b800cec));try{w=-109252==w.getUTCFullYear()&&0===w.getUTCMonth()&&1===w.getUTCDate()&&10==w.getUTCHours()&&37==w.getUTCMinutes()&&6==w.getUTCSeconds()&&708==w.getUTCMilliseconds()}catch(b){}if(!r("json")){var _=r("bug-string-char-index");if(!w)var y=h.floor,S=[0,31,59,90,120,151,181,212,243,273,304,334],I=function(e,t){return S[t]+365*(e-1970)+y((e-1969+(t=+(1<t)))/4)-y((e-1901+t)/100)+y((e-1601+t)/400)};(g=c.hasOwnProperty)||(g=function(e){var t={},i;(t.__proto__=null,t.__proto__={toString:1},t).toString!=p?g=function(e){var t=this.__proto__;e=e in(this.__proto__=null,this);this.__proto__=t;return e}:(i=t.constructor,g=function(e){var t=(this.constructor||i).prototype;return e in this&&!(e in t&&this[e]===t[e])});t=null;return g.call(this,e)});m=function(e,t){var n=0,o,r,a;(o=function(){this.valueOf=0}).prototype.valueOf=0;r=new o;for(a in r)g.call(r,a)&&n++;o=r=null;n?m=2==n?function(e,t){var i={},n="[object Function]"==p.call(e),o;for(o in e)n&&"prototype"==o||g.call(i,o)||!(i[o]=1)||!g.call(e,o)||t(o)}:function(e,t){var i="[object Function]"==p.call(e),n,o;for(n in e)i&&"prototype"==n||!g.call(e,n)||(o="constructor"===n)||t(n);(o||g.call(e,n="constructor"))&&t(n)}:(r="valueOf toString toLocaleString propertyIsEnumerable isPrototypeOf hasOwnProperty constructor".split(" "),m=function(e,t){var n="[object Function]"==p.call(e),o,a=!n&&"function"!=typeof e.constructor&&i[typeof e.hasOwnProperty]&&e.hasOwnProperty||g;for(o in e)n&&"prototype"==o||!a.call(e,o)||t(o);for(n=r.length;o=r[--n];a.call(e,o)&&t(o));});return m(e,t)};if(!r("json-stringify")){var M={92:"\\\\",34:'\\"',8:"\\b",12:"\\f",10:"\\n",13:"\\r",9:"\\t"},k=function(e,t){return("000000"+(t||0)).slice(-e)},U=function(e){for(var t='"',i=0,n=e.length,o=!_||10<n,r=o&&(_?e.split(""):e);i<n;i++){var a=e.charCodeAt(i);switch(a){case 8:case 9:case 10:case 12:case 13:case 34:case 92:t+=M[a];break;default:if(32>a){t+="\\u00"+k(2,a.toString(16));break}t+=o?r[i]:e.charAt(i)}}return t+'"'},C=function(e,t,i,n,o,r,a){var s,c,l,f,h,d,w,b,_;try{s=t[e]}catch(S){}if("object"==typeof s&&s)if(c=p.call(s),"[object Date]"!=c||g.call(s,"toJSON"))"function"==typeof s.toJSON&&("[object Number]"!=c&&"[object String]"!=c&&"[object Array]"!=c||g.call(s,"toJSON"))&&(s=s.toJSON(e));else if(s>-1/0&&s<1/0){if(I){f=y(s/864e5);for(c=y(f/365.2425)+1970-1;I(c+1,0)<=f;c++);for(l=y((f-I(c,0))/30.42);I(c,l+1)<=f;l++);f=1+f-I(c,l);h=(s%864e5+864e5)%864e5;d=y(h/36e5)%24;w=y(h/6e4)%60;b=y(h/1e3)%60;h%=1e3}else c=s.getUTCFullYear(),l=s.getUTCMonth(),f=s.getUTCDate(),d=s.getUTCHours(),w=s.getUTCMinutes(),b=s.getUTCSeconds(),h=s.getUTCMilliseconds();s=(0>=c||1e4<=c?(0>c?"-":"+")+k(6,0>c?-c:c):k(4,c))+"-"+k(2,l+1)+"-"+k(2,f)+"T"+k(2,d)+":"+k(2,w)+":"+k(2,b)+"."+k(3,h)+"Z"}else s=null;i&&(s=i.call(t,e,s));if(null===s)return"null";c=p.call(s);if("[object Boolean]"==c)return""+s;if("[object Number]"==c)return s>-1/0&&s<1/0?""+s:"null";if("[object String]"==c)return U(""+s);if("object"==typeof s){for(e=a.length;e--;)if(a[e]===s)throw u();a.push(s);_=[];t=r;r+=o;if("[object Array]"==c){l=0;for(e=s.length;l<e;l++)c=C(l,s,i,n,o,r,a),_.push(c===v?"null":c);e=_.length?o?"[\n"+r+_.join(",\n"+r)+"\n"+t+"]":"["+_.join(",")+"]":"[]"}else m(n||s,function(e){var t=C(e,s,i,n,o,r,a);t!==v&&_.push(U(e)+":"+(o?" ":"")+t)}),e=_.length?o?"{\n"+r+_.join(",\n"+r)+"\n"+t+"}":"{"+_.join(",")+"}":"{}";a.pop();return e}};n.stringify=function(e,t,n){var o,r,a,s;if(i[typeof t]&&t)if("[object Function]"==(s=p.call(t)))r=t;else if("[object Array]"==s){a={};for(var c=0,l=t.length,f;c<l;f=t[c++],(s=p.call(f),"[object String]"==s||"[object Number]"==s)&&(a[f]=1));}if(n)if("[object Number]"==(s=p.call(n))){if(0<(n-=n%1))for(o="",10<n&&(n=10);o.length<n;o+=" ");}else"[object String]"==s&&(o=10>=n.length?n:n.slice(0,10));return C("",(f={},f[""]=e,f),r,a,o,"",[])}}if(!r("json-parse")){var R=s.fromCharCode,D={92:"\\",34:'"',47:"/",98:"\b",116:"\t",110:"\n",102:"\f",114:"\r"},x,T,O=function(){x=T=null;throw f()},E=function(){for(var e=T,t=e.length,i,n,o,r,a;x<t;)switch(a=e.charCodeAt(x),a){case 9:case 10:case 13:case 32:x++;break;case 123:case 125:case 91:case 93:case 58:case 44:return i=_?e.charAt(x):e[x],x++,i;case 34:i="@";for(x++;x<t;)if(a=e.charCodeAt(x),32>a)O();else if(92==a)switch(a=e.charCodeAt(++x),a){case 92:case 34:case 47:case 98:case 116:case 110:case 102:case 114:i+=D[a];x++;break;case 117:n=++x;for(o=x+4;x<o;x++)a=e.charCodeAt(x),48<=a&&57>=a||97<=a&&102>=a||65<=a&&70>=a||O();i+=R("0x"+e.slice(n,x));break;default:O()}else{if(34==a)break;a=e.charCodeAt(x);for(n=x;32<=a&&92!=a&&34!=a;)a=e.charCodeAt(++x);i+=e.slice(n,x)}if(34==e.charCodeAt(x))return x++,i;O();default:n=x;45==a&&(r=!0,a=e.charCodeAt(++x));if(48<=a&&57>=a){for(48==a&&(a=e.charCodeAt(x+1),48<=a&&57>=a)&&O();x<t&&(a=e.charCodeAt(x),48<=a&&57>=a);x++);if(46==e.charCodeAt(x)){for(o=++x;o<t&&(a=e.charCodeAt(o),48<=a&&57>=a);o++);o==x&&O();x=o}a=e.charCodeAt(x);if(101==a||69==a){a=e.charCodeAt(++x);43!=a&&45!=a||x++;for(o=x;o<t&&(a=e.charCodeAt(o),48<=a&&57>=a);o++);o==x&&O();x=o}return+e.slice(n,x)}r&&O();if("true"==e.slice(x,x+4))return x+=4,!0;if("false"==e.slice(x,x+5))return x+=5,!1;if("null"==e.slice(x,x+4))return x+=4,null;O()}return"$"},j=function(e){var t,i;"$"==e&&O();if("string"==typeof e){if("@"==(_?e.charAt(0):e[0]))return e.slice(1);if("["==e){for(t=[];;i||(i=!0)){e=E();if("]"==e)break;i&&(","==e?(e=E(),"]"==e&&O()):O());","==e&&O();t.push(j(e))}return t}if("{"==e){for(t={};;i||(i=!0)){e=E();if("}"==e)break;i&&(","==e?(e=E(),"}"==e&&O()):O());","!=e&&"string"==typeof e&&"@"==(_?e.charAt(0):e[0])&&":"==E()||O();t[e.slice(1)]=j(E())}return t}O()}return e},G=function(e,t,i){i=B(e,t,i);i===v?delete e[t]:e[t]=i},B=function(e,t,i){var n=e[t],o;if("object"==typeof n&&n)if("[object Array]"==p.call(n))for(o=n.length;o--;)G(n,o,i);else m(n,function(e){G(n,e,i)});return i.call(e,t,n)};n.parse=function(e,t){var i,n;x=0;T=""+e;i=j(E());"$"!=E()&&O();x=T=null;return t&&"[object Function]"==p.call(t)?B((n={},n[""]=i,n),"",t):i}}}n.runInContext=e;return n}var t="function"==typeof define&&define.amd,i={"function":!0,object:!0},n=i[typeof exports]&&exports&&!exports.nodeType&&exports,o=i[typeof window]&&window||this,r=n&&i[typeof module]&&module&&!module.nodeType&&"object"==typeof global&&global;!r||r.global!==r&&r.window!==r&&r.self!==r||(o=r);if(n&&!t)e(o,n);else{var a=o.JSON,s=o.JSON3,c=!1,l=e(o,o.JSON3={noConflict:function(){c||(c=!0,o.JSON=a,o.JSON3=s,a=s=null);return l}});o.JSON={parse:l.parse,stringify:l.stringify}}t&&define("URS-JSON3",function(){return l})}).call(this);