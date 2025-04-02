function _defineProperty(e,r,t){return(r=_toPropertyKey(r))in e?Object.defineProperty(e,r,{value:t,enumerable:!0,configurable:!0,writable:!0}):e[r]=t,e}function _toPropertyKey(t){var i=_toPrimitive(t,"string");return"symbol"==typeof i?i:i+""}function _toPrimitive(t,r){if("object"!=typeof t||!t)return t;var e=t[Symbol.toPrimitive];if(void 0!==e){var i=e.call(t,r||"default");if("object"!=typeof i)return i;throw new TypeError("@@toPrimitive must return a primitive value.")}return("string"===r?String:Number)(t)}var g;g||(g=typeof Module != 'undefined' ? Module : {});var ba=Object.assign({},g),m="",ca;"undefined"!=typeof document&&document.currentScript&&(m=document.currentScript.src);m=m.startsWith("blob:")?"":m.substr(0,m.replace(/[?#].*/,"").lastIndexOf("/")+1);ca=a=>fetch(a,{credentials:"same-origin"}).then(b=>b.ok?b.arrayBuffer():Promise.reject(Error(b.status+" : "+b.url)));var da=g.print||console.log.bind(console),p=g.printErr||console.error.bind(console);Object.assign(g,ba);ba=null;var ea=g.wasmBinary,fa,ha=!1,ia,r,t,v,ja,x,y,ka,la;function ma(){var a=fa.buffer;g.HEAP8=r=new Int8Array(a);g.HEAP16=v=new Int16Array(a);g.HEAPU8=t=new Uint8Array(a);g.HEAPU16=ja=new Uint16Array(a);g.HEAP32=x=new Int32Array(a);g.HEAPU32=y=new Uint32Array(a);g.HEAPF32=ka=new Float32Array(a);g.HEAPF64=la=new Float64Array(a)}var na=[],oa=[],pa=[],qa=[];function ra(){var a=g.preRun.shift();na.unshift(a)}var A=0,sa=null,B=null;function ta(){var _g$monitorRunDependen,_g;A++;(_g$monitorRunDependen=(_g=g).monitorRunDependencies)===null||_g$monitorRunDependen===void 0||_g$monitorRunDependen.call(_g,A)}function ua(){var _g$monitorRunDependen2,_g2;A--;(_g$monitorRunDependen2=(_g2=g).monitorRunDependencies)===null||_g$monitorRunDependen2===void 0||_g$monitorRunDependen2.call(_g2,A);if(0==A&&(null!==sa&&(clearInterval(sa),sa=null),B)){var a=B;B=null;a()}}function va(a){var _g$onAbort,_g3;(_g$onAbort=(_g3=g).onAbort)===null||_g$onAbort===void 0||_g$onAbort.call(_g3,a);a="Aborted("+a+")";p(a);ha=!0;throw new WebAssembly.RuntimeError(a+". Build with -sASSERTIONS for more info.")}var wa=a=>a.startsWith("data:application/octet-stream;base64,"),xa;function ya(a){if(a==xa&&ea)return new Uint8Array(ea);throw"both async and sync fetching of the wasm failed"}function za(a){return ea?Promise.resolve().then(()=>ya(a)):ca(a).then(b=>new Uint8Array(b),()=>ya(a))}function Aa(a,b,c){return za(a).then(d=>WebAssembly.instantiate(d,b)).then(c,d=>{p(`failed to asynchronously prepare wasm: ${d}`);va(d)})}function Ba(a,b){var c=xa;ea||"function"!=typeof WebAssembly.instantiateStreaming||wa(c)||"function"!=typeof fetch?Aa(c,a,b):fetch(c,{credentials:"same-origin"}).then(d=>WebAssembly.instantiateStreaming(d,a).then(b,function(f){p(`wasm streaming compile failed: ${f}`);p("falling back to ArrayBuffer instantiation");return Aa(c,a,b)}))}class Ca{constructor(a){_defineProperty(this,"name","ExitStatus");this.message=`Program terminated with exit(${a})`;this.status=a}}var Da=a=>{for(;0<a.length;)a.shift()(g)},Ea=g.noExitRuntime||!0,Fa=new TextDecoder,D=(a,b)=>{if(!a)return"";b=a+b;for(var c=a;!(c>=b)&&t[c];)++c;return Fa.decode(t.subarray(a,c))};class Ga{constructor(a){this.va=a-24}}var Ha=0,Ia=0,F;function Ja(a){var b=G.wa[a];b.target.removeEventListener(b.ya,b.Na,b.Ja);G.wa.splice(a,1)}function Ka(){if(navigator.userActivation?navigator.userActivation.isActive:G.Fa&&G.Va.Ua){var a=G.xa;G.xa=[];for(var b of a)b.Ha(...b.Ka)}}function La(a){if(!a.target)return-4;if(a.La)a.Na=function(c){++G.Fa;G.Va=a;Ka();a.Oa(c);Ka();--G.Fa},a.target.addEventListener(a.ya,a.Na,a.Ja),G.wa.push(a);else for(var b=0;b<G.wa.length;++b)G.wa[b].target==a.target&&G.wa[b].ya==a.ya&&Ja(b--);return 0}var G={$a:0,ib:0,Ga:0,Da:0,vb:0,tb:0,gb:0,eb:0,lb:0,cb:0,hb:0,mb:0,ub:0,sb:0,ob(){for(;G.wa.length;)Ja(G.wa.length-1);G.xa=[]},Fa:0,xa:[],bb(a,b,c){function d(h,e){if(h.length!=e.length)return!1;for(var k in h)if(h[k]!=e[k])return!1;return!0}for(var f of G.xa)if(f.Ha==a&&d(f.Ka,c))return;G.xa.push({Ha:a,Pa:b,Ka:c});G.xa.sort((h,e)=>h.Pa<e.Pa)},qb(a){G.xa=G.xa.filter(b=>b.Ha!=a)},wa:[],pb:(a,b)=>{for(var c=0;c<G.wa.length;++c)G.wa[c].target!=a||b&&b!=G.wa[c].ya||Ja(c--)},jb(a){return a?a==window?"#window":a==screen?"#screen":(a===null||a===void 0?void 0:a.nodeName)||"":""},fullscreenEnabled(){return document.fullscreenEnabled||document.webkitFullscreenEnabled}},Ma=[0,document,window],Na=a=>{a=2<a?D(a):a;return Ma[a]||document.querySelector(a)},H=(a,b,c)=>{var d=t;if(!(0<c))return 0;var f=b;c=b+c-1;for(var h=0;h<a.length;++h){var e=a.charCodeAt(h);if(55296<=e&&57343>=e){var k=a.charCodeAt(++h);e=65536+((e&1023)<<10)|k&1023}if(127>=e){if(b>=c)break;d[b++]=e}else{if(2047>=e){if(b+1>=c)break;d[b++]=192|e>>6}else{if(65535>=e){if(b+2>=c)break;d[b++]=224|e>>12}else{if(b+3>=c)break;d[b++]=240|e>>18;d[b++]=128|e>>12&63}d[b++]=128|e>>6&63}d[b++]=128|e&63}}d[b]=0;return b-f},Oa=(a,b,c,d,f,h)=>{G.Ga||(G.Ga=I(160));a={target:Na(a),ya:h,La:d,Oa:e=>{var k=G.Ga;la[k>>3]=e.timeStamp;var l=k>>2;x[l+2]=e.location;r[k+12]=e.ctrlKey;r[k+13]=e.shiftKey;r[k+14]=e.altKey;r[k+15]=e.metaKey;r[k+16]=e.repeat;x[l+5]=e.charCode;x[l+6]=e.keyCode;x[l+7]=e.which;H(e.key||"",k+32,32);H(e.code||"",k+64,32);H(e.char||"",k+96,32);H(e.locale||"",k+128,32);F.get(d)(f,k,b)&&e.preventDefault()},Ja:c};return La(a)},Pa=(a,b,c,d,f,h)=>{G.Da||(G.Da=I(64));a=Na(a);return La({target:a,Ua:"mousemove"!=h&&"mouseenter"!=h&&"mouseleave"!=h,ya:h,La:d,Oa:function(){let e=arguments.length>0&&arguments[0]!==undefined?arguments[0]:event;var k=a,l=G.Da;la[l>>3]=e.timeStamp;var q=l>>2;x[q+2]=e.screenX;x[q+3]=e.screenY;x[q+4]=e.clientX;x[q+5]=e.clientY;r[l+24]=e.ctrlKey;r[l+25]=e.shiftKey;r[l+26]=e.altKey;r[l+27]=e.metaKey;v[2*q+14]=e.button;v[2*q+15]=e.buttons;x[q+8]=e.movementX;x[q+9]=e.movementY;k=0>Ma.indexOf(k)?k.getBoundingClientRect():{left:0,top:0};x[q+10]=e.clientX-(k.left|0);x[q+11]=e.clientY-(k.top|0);F.get(d)(f,G.Da,b)&&e.preventDefault()},Ja:c})};function Qa(a){var b=O,c=b.Sa.pop()||b.va.length;b.va[c]=a;return c}class Ra{constructor(){_defineProperty(this,"va",[void 0]);_defineProperty(this,"Sa",[])}get(a){return this.va[a]}has(a){return void 0!==this.va[a]}}var O,P;function Sa(a,b,c,d,f){function h(){var w=0,C=0;n.response&&M&&0===y[a+12>>2]&&(C=n.response.byteLength);0<C&&(w=I(C),t.set(new Uint8Array(n.response),w));y[a+12>>2]=w;Q(a+16,C);Q(a+24,0);(w=n.response?n.response.byteLength:0)&&Q(a+32,w);v[a+40>>1]=n.readyState;v[a+42>>1]=n.status;n.statusText&&H(n.statusText,a+44,64)}var e=y[a+8>>2];if(e){var k=D(e),l=a+108,q=D(l+0);q||(q="GET");var z=y[l+56>>2],E=y[l+68>>2],U=y[l+72>>2];e=y[l+76>>2];var N=y[l+80>>2],R=y[l+84>>2],K=y[l+88>>2],J=y[l+52>>2],M=!!(J&1),Y=!!(J&2);J=!!(J&64);E=E?D(E):void 0;U=U?D(U):void 0;var n=new XMLHttpRequest;n.withCredentials=!!t[l+60];n.open(q,k,!J,E,U);J||(n.timeout=z);n.va=k;n.responseType="arraybuffer";N&&(k=D(N),n.overrideMimeType(k));if(e)for(;;){l=y[e>>2];if(!l)break;k=y[e+4>>2];if(!k)break;e+=8;l=D(l);k=D(k);n.setRequestHeader(l,k)}var L=Qa(n);y[a>>2]=L;e=R&&K?t.slice(R,R+K):null;n.onload=w=>{O.has(L)&&(h(),200<=n.status&&300>n.status?b===null||b===void 0?void 0:b(a,n,w):c===null||c===void 0?void 0:c(a,n,w))};n.onerror=w=>{O.has(L)&&(h(),c===null||c===void 0?void 0:c(a,n,w))};n.ontimeout=w=>{O.has(L)&&(c===null||c===void 0?void 0:c(a,n,w))};n.onprogress=w=>{if(O.has(L)){var C=M&&Y&&n.response?n.response.byteLength:0,u=0;0<C&&M&&Y&&(u=I(C),t.set(new Uint8Array(n.response),u));y[a+12>>2]=u;Q(a+16,C);Q(a+24,w.loaded-C);Q(a+32,w.total);v[a+40>>1]=n.readyState;3<=n.readyState&&0===n.status&&0<w.loaded&&(n.status=200);v[a+42>>1]=n.status;n.statusText&&H(n.statusText,a+44,64);d===null||d===void 0||d(a,n,w);u&&Ta(u)}};n.onreadystatechange=w=>{O.has(L)&&(v[a+40>>1]=n.readyState,2<=n.readyState&&(v[a+42>>1]=n.status),f===null||f===void 0?void 0:f(a,n,w))};try{n.send(e)}catch(w){c===null||c===void 0||c(a,n,w)}}else c(a,0,"no url specified!")}var Ua=a=>{if(!(a instanceof Ca||"unwind"==a))throw a},Va=a=>{var _g$onExit,_g4;ia=a;Ea||((_g$onExit=(_g4=g).onExit)!==null&&_g$onExit!==void 0&&_g$onExit.call(_g4,a),ha=!0);throw new Ca(a)},Q=(a,b)=>{y[a>>2]=b;y[a+4>>2]=(b-y[a>>2])/4294967296};function Wa(a,b,c,d){var f=P;if(f){var h=y[a+108+64>>2];h||(h=y[a+8>>2]);var e=D(h);try{var k=f.transaction(["FILES"],"readwrite").objectStore("FILES").put(b,e);k.onsuccess=()=>{v[a+40>>1]=4;v[a+42>>1]=200;H("OK",a+44,64);c(a,0,e)};k.onerror=l=>{v[a+40>>1]=4;v[a+42>>1]=413;H("Payload Too Large",a+44,64);d(a,0,l)}}catch(l){d(a,0,l)}}else d(a,0,"IndexedDB not available!")}function Xa(a,b,c){var d=P;if(d){var f=y[a+108+64>>2];f||(f=y[a+8>>2]);f=D(f);try{var h=d.transaction(["FILES"],"readonly").objectStore("FILES").get(f);h.onsuccess=e=>{if(e.target.result){e=e.target.result;var k=e.byteLength||e.length,l=I(k);t.set(new Uint8Array(e),l);y[a+12>>2]=l;Q(a+16,k);Q(a+24,0);Q(a+32,k);v[a+40>>1]=4;v[a+42>>1]=200;H("OK",a+44,64);b(a,0,e)}else v[a+40>>1]=4,v[a+42>>1]=404,H("Not Found",a+44,64),c(a,0,"no data")};h.onerror=e=>{v[a+40>>1]=4;v[a+42>>1]=404;H("Not Found",a+44,64);c(a,0,e)}}catch(e){c(a,0,e)}}else c(a,0,"IndexedDB not available!")}function Ya(a,b,c){var d=P;if(d){var f=y[a+108+64>>2];f||(f=y[a+8>>2]);f=D(f);try{var h=d.transaction(["FILES"],"readwrite").objectStore("FILES").delete(f);h.onsuccess=e=>{e=e.target.result;y[a+12>>2]=0;Q(a+16,0);Q(a+24,0);Q(a+32,0);v[a+40>>1]=4;v[a+42>>1]=200;H("OK",a+44,64);b(a,0,e)};h.onerror=e=>{v[a+40>>1]=4;v[a+42>>1]=404;H("Not Found",a+44,64);c(a,0,e)}}catch(e){c(a,0,e)}}else c(a,0,"IndexedDB not available!")}var S,Za=1,$a=[],T=[],ab=[],V=[],W=[],bb=[],cb=[],db=a=>{for(var b=Za++,c=a.length;c<b;c++)a[c]=null;return b},eb=(a,b,c,d)=>{for(var f=0;f<a;f++){var h=S[c](),e=h&&db(d);h&&(h.name=e,d[e]=h);x[b+4*f>>2]=e}},gb=(a,b)=>{a.va||(a.va=a.getContext,a.getContext=function(d,f){f=a.va(d,f);return"webgl"==d==f instanceof WebGLRenderingContext?f:null});var c=a.getContext("webgl2",b);return c?fb(c,b):0},fb=(a,b)=>{var c=db(cb);b={handle:c,attributes:b,version:b.Ya,Ta:a};a.canvas&&(a.canvas.Za=b);cb[c]=b;return c},hb,ib=["default","low-power","high-performance"],jb=[null,[],[]],kb=a=>"]"==a.slice(-1)&&a.lastIndexOf("["),lb=a=>{a-=5120;return 0==a?r:1==a?t:2==a?v:4==a?x:6==a?ka:5==a||28922==a||28520==a||30779==a||30782==a?y:ja},mb=a=>{var b=S.Wa,c=b.za[a];"number"==typeof c&&(b.za[a]=c=S.getUniformLocation(b,b.Qa[a]+(0<c?`[${c}]`:"")));return c};O=new Ra;ta();(function(a,b){try{var c=indexedDB.open("emscripten_filesystem",1)}catch(d){b(d);return}c.onupgradeneeded=d=>{d=d.target.result;d.objectStoreNames.contains("FILES")&&d.deleteObjectStore("FILES");d.createObjectStore("FILES")};c.onsuccess=d=>a(d.target.result);c.onerror=b})(a=>{P=a;ua()},()=>{P=!1;ua()});var nb={e:(a,b,c,d)=>{va(`Assertion failed: ${D(a)}, at: `+[b?D(b):"unknown filename",c,d?D(d):"unknown function"])},s:(a,b,c)=>{var d=new Ga(a);y[d.va+16>>2]=0;y[d.va+4>>2]=b;y[d.va+8>>2]=c;Ha=a;Ia++;throw Ha},M:()=>{va("")},S:function(a){if(O.has(a)){var b=O.get(a),c=O;c.va[a]=void 0;c.Sa.push(a);0<b.readyState&&4>b.readyState&&b.abort()}},N:()=>1,O:()=>Date.now(),Q:()=>performance.now(),V:()=>!0,ea:(a,b)=>{function c(d){F.get(a)(d,b)&&requestAnimationFrame(c)}return requestAnimationFrame(c)},P:a=>{var b=t.length;a>>>=0;if(2147483648<a)return!1;for(var c=1;4>=c;c*=2){var d=b*(1+.2/c);d=Math.min(d,a+100663296);a:{d=(Math.min(2147483648,65536*Math.ceil(Math.max(a,d)/65536))-fa.buffer.byteLength+65535)/65536|0;try{fa.grow(d);ma();var f=1;break a}catch(h){}f=void 0}if(f)return!0}return!1},oa:(a,b,c)=>{a=Na(a);if(!a)return-4;a.width=b;a.height=c;return 0},ba:(a,b,c,d)=>Oa(a,b,c,d,2,"keydown"),U:(a,b,c,d)=>Oa(a,b,c,d,3,"keyup"),L:(a,b,c,d)=>Pa(a,b,c,d,5,"mousedown"),J:(a,b,c,d)=>Pa(a,b,c,d,8,"mousemove"),K:(a,b,c,d)=>Pa(a,b,c,d,6,"mouseup"),T:function(a,b,c,d,f){function h(u){if(U)u();else if(!ha)try{if(u(),!Ea)try{ia=u=ia,Va(u)}catch(Z){Ua(Z)}}catch(Z){Ua(Z)}}var e=a+108,k=y[e+36>>2],l=y[e+40>>2],q=y[e+44>>2],z=y[e+48>>2],E=y[e+52>>2],U=!!(E&64),N=u=>{h(()=>{k?F.get(k)(u):b===null||b===void 0?void 0:b(u)})},R=u=>{h(()=>{q?F.get(q)(u):d===null||d===void 0?void 0:d(u)})},K=u=>{h(()=>{l?F.get(l)(u):c===null||c===void 0?void 0:c(u)})},J=u=>{h(()=>{z?F.get(z)(u):f===null||f===void 0?void 0:f(u)})},M=u=>{Sa(u,N,K,R,J)},Y=(u,Z)=>{Wa(u,Z.response,aa=>{h(()=>{k?F.get(k)(aa):b===null||b===void 0?void 0:b(aa)})},aa=>{h(()=>{k?F.get(k)(aa):b===null||b===void 0?void 0:b(aa)})})},n=u=>{Sa(u,Y,K,R,J)},L=D(e+0),w=!!(E&16),C=!!(E&4);E=!!(E&32);if("EM_IDB_STORE"===L)M=y[e+84>>2],Wa(a,t.slice(M,M+y[e+88>>2]),N,K);else if("EM_IDB_DELETE"===L)Ya(a,N,K);else if(w){if(E)return 0;Sa(a,C?Y:N,K,R,J)}else Xa(a,N,E?K:C?n:M);return a},na:(a,b)=>{var c=b>>2;b={alpha:!!r[b+0],depth:!!r[b+1],stencil:!!r[b+2],antialias:!!r[b+3],premultipliedAlpha:!!r[b+4],preserveDrawingBuffer:!!r[b+5],powerPreference:ib[x[c+2]],failIfMajorPerformanceCaveat:!!r[b+12],Ya:x[c+4],kb:x[c+5],fb:r[b+24],Xa:r[b+25],nb:x[c+7],rb:r[b+32]};a=Na(a);return!a||b.Xa?0:gb(a,b)},ma:a=>{var _hb;hb=cb[a];g.ab=S=(_hb=hb)===null||_hb===void 0?void 0:_hb.Ta;return!a||S?0:-5},u:(a,b,c,d)=>{for(var f=0,h=0;h<c;h++){var e=y[b>>2],k=y[b+4>>2];b+=8;for(var l=0;l<k;l++){var q=t[e+l],z=jb[a];if(0===q||10===q){for(q=0;z[q]&&!(NaN<=q);)++q;q=Fa.decode(z.buffer?z.subarray(0,q):new Uint8Array(z.slice(0,q)));(1===a?da:p)(q);z.length=0}else z.push(q)}f+=k}y[d>>2]=f;return 0},ka:a=>S.activeTexture(a),x:(a,b)=>{S.attachShader(T[a],W[b])},c:(a,b)=>{35051==a?S.Ma=b:35052==a&&(S.Ea=b);S.bindBuffer(a,$a[b])},z:(a,b)=>{S.bindFramebuffer(a,ab[b])},b:(a,b)=>{S.bindTexture(a,V[b])},i:a=>{S.bindVertexArray(bb[a])},o:a=>S.blendEquation(a),p:(a,b)=>S.blendFunc(a,b),k:(a,b,c,d)=>{c&&b?S.bufferData(a,t,d,c,b):S.bufferData(a,b,d)},C:a=>S.clear(a),D:(a,b,c,d)=>S.clearColor(a,b,c,d),Z:a=>{S.compileShader(W[a])},da:()=>{var a=db(T),b=S.createProgram();b.name=a;b.Ca=b.Aa=b.Ba=0;b.Ia=1;T[a]=b;return a},$:a=>{var b=db(W);W[b]=S.createShader(a);return b},q:(a,b)=>{for(var c=0;c<a;c++){var d=x[b+4*c>>2],f=$a[d];f&&(S.deleteBuffer(f),f.name=0,$a[d]=null,d==S.Ma&&(S.Ma=0),d==S.Ea&&(S.Ea=0))}},E:(a,b)=>{for(var c=0;c<a;++c){var d=x[b+4*c>>2],f=ab[d];f&&(S.deleteFramebuffer(f),f.name=0,ab[d]=null)}},j:a=>{if(a){var b=T[a];b&&(S.deleteProgram(b),b.name=0,T[a]=null)}},d:a=>{if(a){var b=W[a];b&&(S.deleteShader(b),W[a]=null)}},ga:(a,b)=>{for(var c=0;c<a;c++){var d=x[b+4*c>>2],f=V[d];f&&(S.deleteTexture(f),f.name=0,V[d]=null)}},H:(a,b)=>{for(var c=0;c<a;c++){var d=x[b+4*c>>2];S.deleteVertexArray(bb[d]);bb[d]=null}},I:a=>S.disable(a),B:(a,b,c,d)=>{S.drawArraysInstanced(a,b,c,d)},ia:(a,b,c,d)=>{S.drawElements(a,b,c,d)},la:a=>S.enable(a),f:a=>{S.enableVertexAttribArray(a)},fa:(a,b,c,d,f)=>{S.framebufferTexture2D(a,b,c,V[d],f)},m:(a,b)=>{eb(a,b,"createBuffer",$a)},X:(a,b)=>{eb(a,b,"createFramebuffer",ab)},n:(a,b)=>{eb(a,b,"createTexture",V)},y:(a,b)=>{eb(a,b,"createVertexArray",bb)},h:(a,b)=>S.getAttribLocation(T[a],D(b)),aa:(a,b,c,d)=>{a=S.getProgramInfoLog(T[a]);b=0<b&&d?H(a,d,b):0;c&&(x[c>>2]=b)},w:(a,b,c)=>{if(c&&!(a>=Za))if(a=T[a],35716==b)x[c>>2]=S.getProgramInfoLog(a).length+1;else if(35719==b){if(!a.Ca){var d=S.getProgramParameter(a,35718);for(b=0;b<d;++b)a.Ca=Math.max(a.Ca,S.getActiveUniform(a,b).name.length+1)}x[c>>2]=a.Ca}else if(35722==b){if(!a.Aa)for(d=S.getProgramParameter(a,35721),b=0;b<d;++b)a.Aa=Math.max(a.Aa,S.getActiveAttrib(a,b).name.length+1);x[c>>2]=a.Aa}else if(35381==b){if(!a.Ba)for(d=S.getProgramParameter(a,35382),b=0;b<d;++b)a.Ba=Math.max(a.Ba,S.getActiveUniformBlockName(a,b).length+1);x[c>>2]=a.Ba}else x[c>>2]=S.getProgramParameter(a,b)},Y:(a,b,c,d)=>{a=S.getShaderInfoLog(W[a]);b=0<b&&d?H(a,d,b):0;c&&(x[c>>2]=b)},v:(a,b,c)=>{c&&(35716==b?(a=S.getShaderInfoLog(W[a]),x[c>>2]=a?a.length+1:0):35720==b?(a=S.getShaderSource(W[a]),x[c>>2]=a?a.length+1:0):x[c>>2]=S.getShaderParameter(W[a],b))},r:(a,b)=>{b=D(b);if(a=T[a]){var c=a,d=c.za,f=c.Ra,h;if(!d){c.za=d={};c.Qa={};var e=S.getProgramParameter(c,35718);for(h=0;h<e;++h){var k=S.getActiveUniform(c,h);var l=k.name;k=k.size;var q=kb(l);q=0<q?l.slice(0,q):l;var z=c.Ia;c.Ia+=k;f[q]=[k,z];for(l=0;l<k;++l)d[z]=l,c.Qa[z++]=q}}c=a.za;d=0;f=b;h=kb(b);0<h&&(d=parseInt(b.slice(h+1))>>>0,f=b.slice(0,h));if((f=a.Ra[f])&&d<f[0]&&(d+=f[1],c[d]=c[d]||S.getUniformLocation(a,b)))return d}return-1},ca:a=>{a=T[a];S.linkProgram(a);a.za=0;a.Ra={}},_:(a,b,c,d)=>{for(var f="",h=0;h<b;++h)f+=D(y[c+4*h>>2],d?y[d+4*h>>2]:void 0);S.shaderSource(W[a],f)},A:(a,b,c,d,f,h,e,k,l)=>{if(S.Ea)S.texImage2D(a,b,c,d,f,h,e,k,l);else if(l){var q=lb(k);l>>>=31-Math.clz32(q.BYTES_PER_ELEMENT);S.texImage2D(a,b,c,d,f,h,e,k,q,l)}else{if(l){q=lb(k);var z=f*(d*({5:3,6:4,8:2,29502:3,29504:4,26917:2,26918:2,29846:3,29847:4}[e-6402]||1)*q.BYTES_PER_ELEMENT+4-1&-4);l=q.subarray(l>>>31-Math.clz32(q.BYTES_PER_ELEMENT),l+z>>>31-Math.clz32(q.BYTES_PER_ELEMENT))}else l=null;S.texImage2D(a,b,c,d,f,h,e,k,l)}},a:(a,b,c)=>S.texParameteri(a,b,c),ja:(a,b)=>{S.uniform1i(mb(a),b)},F:(a,b,c)=>{S.uniform2f(mb(a),b,c)},G:a=>{a=T[a];S.useProgram(a);S.Wa=a},l:(a,b)=>{S.vertexAttribDivisor(a,b)},g:(a,b,c,d,f,h)=>{S.vertexAttribPointer(a,b,c,!!d,f,h)},t:(a,b,c,d)=>S.viewport(a,b,c,d),W:function(a,b,c,d){var f=document.createElement("canvas");f.width=b;f.height=c;b=f.getContext("2d");b.font=a+"px "+D(d);b.textBaseline="middle";b.globalAlpha=1;b.fillStyle="white";window["gCanvas"+a]=f;window["gCanvasCtx"+a]=b},R:function(a,b,c,d){var f=new Image;f.onload=()=>{y[c>>2]=f.width;y[d>>2]=f.height;S.bindTexture(S.TEXTURE_2D,V[a]);S.texImage2D(S.TEXTURE_2D,0,S.RGBA,S.RGBA,S.UNSIGNED_BYTE,f)};f.src=D(b)},ha:function(a,b){var c=window["gCanvas"+a];a=window["gCanvasCtx"+a];a.clearRect(0,0,c.width,c.height);b=D(b);var d=a.measureText(b).width;a.fillText(b,0,c.height/2);S.texImage2D(S.TEXTURE_2D,0,S.RGBA,S.RGBA,S.UNSIGNED_BYTE,c);return d}},X=function(_xa){function a(c){X=c.exports;fa=X.pa;ma();F=X.ra;oa.unshift(X.qa);ua();return X}var b={a:nb};ta();if(g.instantiateWasm)try{return g.instantiateWasm(b,a)}catch(c){return p(`Module.instantiateWasm callback failed with error: ${c}`),!1}(_xa=xa)!==null&&_xa!==void 0?_xa:xa=wa("src1.wasm")?"src1.wasm":g.locateFile?g.locateFile("src1.wasm",m):m+"src1.wasm";Ba(b,function(c){a(c.instance)});return{}}(),ob=g._main=(a,b)=>(ob=g._main=X.sa)(a,b),I=a=>(I=X.ta)(a),Ta=a=>(Ta=X.ua)(a),pb;B=function qb(){pb||rb();pb||(B=qb)};function rb(){function a(){if(!pb&&(pb=!0,g.calledRun=!0,!ha)){var _g$onRuntimeInitializ,_g5;Da(oa);Da(pa);(_g$onRuntimeInitializ=(_g5=g).onRuntimeInitialized)===null||_g$onRuntimeInitializ===void 0||_g$onRuntimeInitializ.call(_g5);if(sb){var b=ob;try{var c=b(0,0);ia=c;Va(c)}catch(d){Ua(d)}}if(g.postRun)for("function"==typeof g.postRun&&(g.postRun=[g.postRun]);g.postRun.length;)b=g.postRun.shift(),qa.unshift(b);Da(qa)}}if(!(0<A)){if(g.preRun)for("function"==typeof g.preRun&&(g.preRun=[g.preRun]);g.preRun.length;)ra();Da(na);0<A||(g.setStatus?(g.setStatus("Running..."),setTimeout(()=>{setTimeout(()=>g.setStatus(""),1);a()},1)):a())}}if(g.preInit)for("function"==typeof g.preInit&&(g.preInit=[g.preInit]);0<g.preInit.length;)g.preInit.pop()();var sb=!0;g.noInitialRun&&(sb=!1);rb();
