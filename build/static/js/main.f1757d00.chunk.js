(this["webpackJsonpsoftware-project"]=this["webpackJsonpsoftware-project"]||[]).push([[0],{13:function(e,t,n){"use strict";n.d(t,"h",(function(){return r})),n.d(t,"f",(function(){return c})),n.d(t,"g",(function(){return o})),n.d(t,"m",(function(){return i})),n.d(t,"G",(function(){return s})),n.d(t,"c",(function(){return u})),n.d(t,"y",(function(){return l})),n.d(t,"b",(function(){return d})),n.d(t,"x",(function(){return j})),n.d(t,"u",(function(){return b})),n.d(t,"O",(function(){return f})),n.d(t,"a",(function(){return O})),n.d(t,"w",(function(){return p})),n.d(t,"d",(function(){return m})),n.d(t,"z",(function(){return g})),n.d(t,"q",(function(){return h})),n.d(t,"K",(function(){return x})),n.d(t,"k",(function(){return v})),n.d(t,"D",(function(){return y})),n.d(t,"j",(function(){return w})),n.d(t,"C",(function(){return C})),n.d(t,"o",(function(){return k})),n.d(t,"I",(function(){return S})),n.d(t,"e",(function(){return E})),n.d(t,"A",(function(){return T})),n.d(t,"t",(function(){return B})),n.d(t,"N",(function(){return A})),n.d(t,"r",(function(){return N})),n.d(t,"L",(function(){return I})),n.d(t,"s",(function(){return L})),n.d(t,"M",(function(){return R})),n.d(t,"E",(function(){return M})),n.d(t,"i",(function(){return P})),n.d(t,"B",(function(){return D})),n.d(t,"l",(function(){return H})),n.d(t,"F",(function(){return _})),n.d(t,"p",(function(){return G})),n.d(t,"J",(function(){return V})),n.d(t,"v",(function(){return F})),n.d(t,"n",(function(){return U})),n.d(t,"H",(function(){return z}));var a=function(e){for(var t=arguments.length,n=new Array(t>1?t-1:0),a=1;a<t;a++)n[a-1]=arguments[a];return function(){for(var t=arguments.length,a=new Array(t),r=0;r<t;r++)a[r]=arguments[r];var c={type:e};return n.forEach((function(e,t){c[n[t]]=a[t]})),c}},r="INCREMENT",c=(a(r),"DECREMENT"),o=(a(c),"GET_SERVERVALUE"),i=(a(o,"payload"),"SERVERVALUE_FULFILLED"),s=a(i,"key","value"),u="CHOOSE_ACTION_START",l=a(u,"point"),d="CHOOSE_DIGIT",j=a(d,"point"),b="UPDATE_SUDOKU",f=a(b),O="BLOCK_HIGHLIGHT",p=a(O,"value"),m="CLEAR_BLOCK_HIGHLIGHT",g=a(m),h="TOGGLE_DIGITBOARD",x=a(h),v="PLAY_ROUND_FORWARD",y=a(v,"payload"),w="PLAY_ROUND_BACKWARD",C=a(w),k="SET_PLACE_VALUE",S=a(k,"value"),E="CLEAR_PLACE_VALUE",T=a(E,"value"),B="TOGGLE_SHOW_UNCHANGEABLE",A=a(B),N="TOGGLE_SHOW_CONFLICT",I=a(N),L="TOGGLE_SHOW_OPTIONNUMBER",R=a(L),M=a("SAVE_GAME"),P="LOAD_GAME",D=a(P,"values","initValues","playHistorys","playRound"),H="SEND_MESSAGE",_=a(H,"message"),G="START_SOCKET",V=a(G),F=a("ADD_MESSAGE","message"),U="SET_LEVEL",z=a(U,"level");a("TOGGLE_PROGRESS")},194:function(e,t,n){},195:function(e,t,n){},244:function(e,t){},247:function(e,t,n){"use strict";n.r(t);var a=n(2),r=n(0),c=n.n(r),o=n(14),i=n.n(o),s=(n(194),n(33)),u=(n(195),n(30)),l=n(50),d=n(7),j=n(89),b=n(13),f={times:0,serverValue:"init serverValue"},O=n(22),p={messages:[],info:{username:"cherro",avatar:""},loading:!1},m=function(e,t){var n=t.length;if(0===n)return[e];for(var a=n-1;a>=0;a--){if(!(t[a].time>e.time)){t[a+1]=e;break}t[a+1]=t[a]}return t.slice(-100)},g=n(17),h=null,x=[[h,h,h,h,h,h,h,h,h],[h,h,h,h,h,h,h,h,h],[h,h,h,h,h,h,h,h,h],[h,h,h,h,h,h,h,h,h],[h,h,h,h,h,h,h,h,h],[h,h,h,h,h,h,h,h,h],[h,h,h,h,h,h,h,h,h],[h,h,h,h,h,h,h,h,h],[h,h,h,h,h,h,h,h,h]],v=[[0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0]];var y=function(e,t){for(var n=[[],[],[],[],[],[],[],[],[]],a=0;a<9;a++)for(var r=0;r<9;r++)null!=t&&t===e[a][r]?n[a][r]=1:n[a][r]=0;return n},w=null,C=[[w,w,w,w,w,w,w,w,w],[w,w,w,w,w,w,w,w,w],[w,w,w,w,w,w,w,w,w],[w,w,w,w,w,w,w,w,w],[w,w,w,w,w,w,w,w,w],[w,w,w,w,w,w,w,w,w],[w,w,w,w,w,w,w,w,w],[w,w,w,w,w,w,w,w,w],[w,w,w,w,w,w,w,w,w]],k=[[w,w,w,w,w,w,w,w,w],[w,w,w,w,w,w,w,w,w],[w,w,w,w,w,w,w,w,w],[w,w,w,w,w,w,w,w,w],[w,w,w,w,w,w,w,w,w],[w,w,w,w,w,w,w,w,w],[w,w,w,w,w,w,w,w,w],[w,w,w,w,w,w,w,w,w],[w,w,w,w,w,w,w,w,w]],S=[[0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0]],E=function(e){for(var t=0;t<9;t++)for(var n=0;n<9;n++)S[t][n]=0,k[t][n]=w,C[t][n]=w;for(;!A(0,1););return function(e){var t,n,a,r=new Array(81);for(t=0;t<81;t++)S[Math.floor(t/9)][t%9]=0,r[t]=t;for(t=0;t<e;t++)a=r[n=Math.floor(81*Math.random())],r[n]=r[t],r[t]=a;for(t=0;t<e;t++)S[Math.floor(r[t]/9)][r[t]%9]=1}(10*(e+1)),function(){var e,t;for(t=0;t<9;t++)for(e=0;e<9;e++)1===S[t][e]?k[t][e]=w:k[t][e]=C[t][e]}(),[k,C]};function T(e,t,n){if(C[t][e]!==w)return!1;var a,r;for(a=0;a<9;a++)if(C[t][a]===n)return!1;for(r=0;r<9;r++)if(C[r][e]===n)return!1;for(r=3*Math.floor(t/3);r<3*Math.floor(t/3)+3;r++)for(a=3*Math.floor(e/3);a<3*Math.floor(e/3)+3;a++)if(C[r][a]===n)return!1;return C[t][e]=n,!0}function B(e,t){C[t][e]=w}function A(e,t){var n,a,r=[1,2,3,4,5,6,7,8,9];for(function(e){var t,n,a;for(t=0;t<9;t++)e[t]=t;for(t=0;t<9;t++)a=e[n=Math.floor(9*Math.random())],e[n]=e[t],e[t]=a}(r),n=0;n<9;n++)if(T(a=r[n],e,t)){if(8===e){if(9===t||A(0,t+1))return!0}else if(A(e+1,t))return!0;B(a,e)}return!1}var N=null,I=[[N,N,N,N,N,N,N,N,N],[N,N,N,N,N,N,N,N,N],[N,N,N,N,N,N,N,N,N],[N,N,N,N,N,N,N,N,N],[N,N,N,N,N,N,N,N,N],[N,N,N,N,N,N,N,N,N],[N,N,N,N,N,N,N,N,N],[N,N,N,N,N,N,N,N,N],[N,N,N,N,N,N,N,N,N]],L=!1,R=!0;function M(e){return Math.floor(e/3)}function P(e){return e%3}var D=function(e){for(var t=0;t<9;t++)for(var n=0;n<9;n++)I[t][n]=N;return L=!1,R=!0,function(e){var t,n,a,r,c;for(t=0;t<9;t++)for(n=0;n<9;n++)for(a=n+1;a<9;a++)e[t][n]===e[t][a]&&(I[t][n]=e[t][n],I[t][a]=e[t][a],L=!0);for(n=0;n<9;n++)for(t=0;t<9;t++)for(a=t+1;a<9;a++)e[t][n]===e[a][n]&&(I[t][n]=e[t][n],I[a][n]=e[a][n],L=!0);for(a=0;a<9;a++)for(r=0;r<9;r++)for(c=r+1;c<9;c++)e[3*M(a)+M(r)][3*P(a)+P(r)]===e[3*M(a)+M(c)][3*P(a)+P(c)]&&(I[3*M(a)+M(r)][3*P(a)+P(r)]=e[3*M(a)+M(r)][3*P(a)+P(r)],I[3*M(a)+M(c)][3*P(a)+P(c)]=e[3*M(a)+M(r)][3*P(a)+P(r)],L=!0)}(e),function(e){var t,n;if(L)R=!1;else for(t=0;t<9;t++)for(n=0;n<9;n++)e[t][n]===N&&(R=!1)}(e),{conflict:L,complete:R,conflictValues:I}},H=E(1)[0],_={values:H,initValues:H.map((function(e){return Object(O.a)(e)})),blockHighlight:v.map((function(e){return Object.assign({},e)})),level:1,point:{x:0,y:0,value:null},highlightPoint:{x:0,y:0,value:null},digitBoard:!1,playRound:0,playHistorys:[],placeValue:null,showUnchangeable:!0,conflictValues:x.map((function(e){return Object(O.a)(e)})),showConflict:!0,complete:!1,showOptionNumber:!0},G=Object(u.combineReducers)({Counters:function(){var e=arguments.length>0&&void 0!==arguments[0]?arguments[0]:f,t=arguments.length>1?arguments[1]:void 0;switch(Object(j.a)(e),t.type){case b.h:return Object(d.a)(Object(d.a)({},e),{},{times:e.times+1});case b.f:return Object(d.a)(Object(d.a)({},e),{},{times:e.times-1});case b.m:return Object(d.a)(Object(d.a)({},e),{},{serverValue:t.key});default:return Object(d.a)(Object(d.a)({},e),{},{times:e.times})}},Game:function(){var e=arguments.length>0&&void 0!==arguments[0]?arguments[0]:_,t=arguments.length>1?arguments[1]:void 0,n=e.values,a=e.level,r=e.digitBoard,c=e.playRound,o=e.playHistorys,i=e.showUnchangeable,s=e.showConflict,u=e.showOptionNumber;switch(t.type){case b.u:var l=E(a),j=Object(g.a)(l,2),f=j[0];j[1];return Object(d.a)(Object(d.a)({},_),{},{values:f,initValues:f.map((function(e){return Object.assign({},e)})),blockHighlight:v.map((function(e){return Object.assign({},e)})),conflictValues:x.map((function(e){return Object(O.a)(e)})),level:a,showUnchangeable:i,showConflict:s,showOptionNumber:u});case b.a:return Object(d.a)(Object(d.a)({},e),{},{blockHighlight:y(n,t.value)});case b.d:var p=v.map((function(e){return Object.assign({},e)}));return Object(d.a)(Object(d.a)({},e),{},{blockHighlight:p});case b.q:return Object(d.a)(Object(d.a)({},e),{},{digitBoard:!r});case b.c:return Object(d.a)(Object(d.a)({},e),{},{point:t.point});case b.b:n[t.point.x][t.point.y]=t.point.value;var m=D(n),h=m.complete,w=m.conflictValues;return Object(d.a)(Object(d.a)({},e),{},{values:n,point:t.point,conflictValues:w,complete:h});case b.j:if(0===c)return Object(d.a)({},e);c>0&&(n[o[c-1].x][o[c-1].y]=o[c-1].from);var C=D(n),k=C.conflictValues;return Object(d.a)(Object(d.a)({},e),{},{values:n,playRound:c-1,playHistorys:o.slice(0,c-1),conflictValues:k});case b.k:return o.push(t.payload),Object(d.a)(Object(d.a)({},e),{},{playRound:c+1,playHistorys:o,blockHighlight:y(n,t.payload.to)});case b.o:return Object(d.a)(Object(d.a)({},e),{},{placeValue:t.value});case b.e:return Object(d.a)(Object(d.a)({},e),{},{placeValue:null});case b.t:return Object(d.a)(Object(d.a)({},e),{},{showUnchangeable:!i});case b.r:return Object(d.a)(Object(d.a)({},e),{},{showConflict:!s});case b.s:return Object(d.a)(Object(d.a)({},e),{},{showOptionNumber:!u});case b.i:var S=D(t.values),T=S.conflictValues;return Object(d.a)(Object(d.a)({},e),{},{values:t.values,initValues:t.initValues,playHistorys:t.playHistorys,playRound:t.playRound,conflictValues:T});case b.n:return Object(d.a)(Object(d.a)({},e),{},{level:t.level});default:return Object(d.a)({},e)}},User:function(){var e=arguments.length>0&&void 0!==arguments[0]?arguments[0]:p,t=arguments.length>1?arguments[1]:void 0;switch(t.type){case b.l:var n=m(t.message,Object(O.a)(e.messages));return Object(d.a)(Object(d.a)({},e),{},{messages:n});default:return Object(d.a)({},e)}}}),V=n(171),F=n(155),U=n(18),z=n(87),W=n(300),K=n(296),$=Object(W.a)((function(e){e.palette,e.shadows,e.spacing,e.breakpoints;return Object(K.a)({success:{backgroundColor:"red"},error:{backgroundColor:"purple"},warning:{backgroundColor:"red"},info:{backgroundColor:"purple"}})})),q=Object(r.memo)((function(e){var t=e.children,n=$();return Object(a.jsx)(z.a,{iconVariant:{success:"\u2705",error:"\u2716\ufe0f",warning:"\u26a0\ufe0f",info:"\u2139\ufe0f"},dense:!1,preventDuplicate:!0,anchorOrigin:{vertical:"bottom",horizontal:"center"},classes:{variantSuccess:n.success,variantError:n.error,variantWarning:n.warning,variantInfo:n.info},children:t})})),J=n(90),Y=n(301),X=n(125),Q=n.n(X),Z=n(256),ee=n(248),te=Object(Z.a)((function(e){var t=e.zIndex;return Object(ee.a)({progress:{position:"fixed",zIndex:t.modal+1,top:0,left:0,width:"100%"},color:{backgroundColor:Q.a[500]},barColor:{backgroundColor:Q.a[100]}})})),ne=Object(r.memo)((function(){var e=te();return Object(a.jsx)(Y.a,{className:e.progress,color:"primary",classes:{colorPrimary:e.color,barColorPrimary:e.barColor}})})),ae=n(8),re=n(110),ce=n(250),oe=n(311),ie=n(312),se=n(255),ue=n(172),le=n(314),de=n(298),je=n(310),be=n(313),fe=n(315),Oe=n(316),pe=n(165),me=n.n(pe),ge=n(163),he=n.n(ge),xe=n(166),ve=n.n(xe),ye=n(164),we=n.n(ye),Ce=n(31),ke=n.n(Ce),Se=n(304),Ee=n(302),Te=n(305),Be=n(254),Ae=n(306),Ne=n(307),Ie=n(156),Le=n.n(Ie),Re=n(157),Me=n.n(Re),Pe=function(e,t){var n=/^#?([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(e)||["","",""],a=parseInt(n[1],16),r=parseInt(n[2],16),c=parseInt(n[3],16);return"rgba(".concat(a,", ").concat(r,", ").concat(c,", ").concat(t,")")},De=Object(Z.a)((function(e){var t=e.transitions,n=e.mixins,a=e.spacing,r=e.breakpoints;return Object(ee.a)({drawerPaper:{position:"sticky",top:0,whiteSpace:"nowrap",overflowX:"hidden",width:200,transition:t.create("width",{easing:t.easing.sharp,duration:t.duration.enteringScreen})},drawerPaperClose:Object(ae.a)({overflowX:"hidden",transition:t.create("width",{easing:t.easing.sharp,duration:t.duration.leavingScreen}),width:a(9)},r.down("md"),{border:0,width:"1px"}),toolbar:Object(d.a)({display:"flex",alignItems:"center",justifyContent:"flex-end",padding:"0 ".concat(a(1),"px")},n.toolbar),icon:{margin:"0 ".concat(a(1),"px")}})})),He=[{icon:Object(a.jsx)(Le.a,{}),text:"dashboard"},{icon:Object(a.jsx)(Me.a,{}),text:"home"}],_e=Object(r.memo)((function(e){var t=e.open,n=De();return Object(a.jsxs)(Ee.a,{variant:"permanent",classes:{paper:ke()(n.drawerPaper,!t&&n.drawerPaperClose)},open:t,children:[Object(a.jsx)("div",{className:n.toolbar}),Object(a.jsx)(Se.a,{}),Object(a.jsx)(Te.a,{children:He.map((function(e,t){var r=e.icon,c=e.text;return Object(a.jsxs)(Be.a,{button:!0,children:[Object(a.jsx)(Ae.a,{className:n.icon,children:r}),Object(a.jsx)(Ne.a,{primary:c})]},"item".concat(t))}))})]})})),Ge=n(177),Ve=Object(W.a)((function(e){var t,n,a=e.palette,r=e.shadows,c=e.spacing,o=e.breakpoints;return Object(K.a)({modalContainer:{display:"flex",alignItems:"center",justifyContent:"center"},modal:(t={display:"flex",flexDirection:"column",position:"absolute",borderRadius:6,backgroundColor:a.background.paper,boxShadow:r[5],padding:c(1),outline:"none"},Object(ae.a)(t,o.down("sm"),{maxWidth:"100%"}),Object(ae.a)(t,"maxHeight","90%"),Object(ae.a)(t,"maxWidth","75%"),t),modalHeader:{background:a.primary.light,borderRadius:3,marginTop:-c(3),marginLeft:c(5),marginRight:c(5),marginBottom:c(1),boxShadow:(n=a.primary.light,"0 12px 20px -10px ".concat(Pe(n,.28),", 0 4px 20px 0px rgba(0, 0, 0, 0.12), 0 7px 8px -5px ").concat(Pe(n,.2))),userSelect:"none"},modalTitle:{color:a.primary.contrastText,textAlign:"center",margin:c(1)},backdrop:{background:a.primary.main}})})),Fe=Object(r.memo)((function(e){var t=e.open,n=e.title,r=e.direction,c=e.children,o=e.onClose,i=Ve(),s="left"===r?"right":"left";return Object(a.jsx)(Ge.a,{open:t,onClose:o,"aria-labelledby":"simple-modal-title","aria-describedby":"simple-modal-description",disableEnforceFocus:!0,children:Object(a.jsx)(ce.a,{direction:t?r||"right":s,in:t,mountOnEnter:!0,unmountOnExit:!0,children:Object(a.jsxs)("div",{className:i.modal,children:[Object(a.jsx)("div",{className:i.modalHeader,children:Object(a.jsx)(re.a,{variant:"h5",className:i.modalTitle,children:n})}),c]})})})})),Ue=Object(W.a)((function(e){var t,n=e.zIndex,a=e.palette,r=e.transitions,c=e.spacing,o=e.breakpoints;return Object(K.a)({appBar:{zIndex:n.drawer+1,background:"".concat(a.primary.main),transition:r.create(["width","margin"],{easing:r.easing.sharp,duration:r.duration.leavingScreen})},appBarShift:{marginLeft:200,width:"calc(100% - ".concat(200,"px)"),transition:r.create(["width","margin"],{easing:r.easing.sharp,duration:r.duration.enteringScreen})},appBarGutters:{paddingRight:0},regular:{minHeight:48,height:48},menuButton:Object(ae.a)({marginLeft:c(2),marginRight:c(4)},o.down("sm"),{marginLeft:0,marginRight:0,padding:c(1)}),rightButtons:{marginLeft:"auto",display:"flex","& button":Object(ae.a)({},o.down("sm"),{padding:c(1)})},hide:{display:"none"},suggestion:{padding:c(2),overflowY:"auto"},collapse:(t={zIndex:n.drawer+1,position:"fixed",top:c(8)},Object(ae.a)(t,o.down("sm"),{top:c(6)}),Object(ae.a)(t,"right",0),t),hideButton:{top:c(0)},hideButtonShowing:{display:"flex",position:"fixed",right:0}})})),ze=n(71),We=n(326),Ke=n(327),$e=n(129),qe=n(322),Je=n(161),Ye=n.n(Je),Xe=n(160),Qe=n.n(Xe),Ze=n(162),et=n.n(Ze),tt=n(308),nt=Object(Z.a)((function(e){var t=e.zIndex;return Object(ee.a)({imageLayer:{maxHeight:"100%"},image:{"&:hover":{cursor:"pointer"},maxWidth:"100%"},imageRoot:{zIndex:4*t.snackbar}})})),at=Object(r.memo)((function(e){var t=e.src,n=nt(),c=Object(r.useState)(!1),o=Object(g.a)(c,2),i=o[0],s=o[1],u=function(){s(!1)};return Object(a.jsxs)(a.Fragment,{children:[Object(a.jsx)("img",{src:t,onClick:function(){s(!0)},className:n.image,alt:"small"}),Object(a.jsx)(tt.a,{open:i,onClose:u,maxWidth:!1,classes:{paper:n.imageLayer,root:n.imageRoot},children:Object(a.jsx)("img",{src:t,onClick:u,className:n.image,alt:"original"})})]})})),rt=Object(Z.a)((function(e){var t,n=e.spacing,a=e.transitions,r=e.breakpoints,c=e.palette,o=e.zIndex;return Object(ee.a)({messenger:(t={display:"flex",flexDirection:"column",margin:n(1),padding:n(1),height:"calc(100vh - 96px)"},Object(ae.a)(t,r.up("sm"),{width:400}),Object(ae.a)(t,r.down("sm"),{height:"75vh"}),Object(ae.a)(t,"flex","1"),t),messages:{flex:"1",overflowY:"auto",padding:n(1),marginBottom:n(1),transition:a.create(["padding"],{easing:a.easing.sharp,duration:a.duration.enteringScreen})},messageContainer:{display:"flex",margin:"".concat(n(1),"px 0")},input:{marginTop:"auto"},inputContent:{display:"flex",alignItems:"flex-end"},textField:{width:"100%"},chipRoot:{height:"auto",padding:n(1),whiteSpace:"normal",background:Pe(c.secondary.light,.3),"& > *":{whiteSpace:"normal"}},myChip:{background:Pe(c.primary.main,.8),color:c.primary.contrastText},myDivider:{background:"white"},messageContent:{wordWrap:"break-word",maxWidth:200,userSelect:"text",cursor:"text",marginTop:n(1)},avatar:{margin:n(1)},my:{flexDirection:"row-reverse"},rightAlign:{marginLeft:"auto"},message:{display:"flex",flexDirection:"column"},hidden:{display:"none"},imageLayer:{maxHeight:"100%"},image:{"&:hover":{cursor:"pointer"},maxWidth:"100%"},imageRoot:{zIndex:4*o.snackbar},closeButton:{marginLeft:"auto"}})})),ct=n(159),ot=n.n(ct),it=Object(r.memo)((function(e){var t=e.SvgProp,n=void 0===t?{}:t,r=e.GProp,c=void 0===r?{}:r,o=e.AvatarId,i=void 0===o?"bind avatar":o,s=ot()(i).match(/<svg[^>]*>(.*)<\/svg>/)[1];return Object(a.jsx)("svg",Object(d.a)(Object(d.a)({},n),{},{viewBox:"0 0 231 231",children:Object(a.jsx)("g",Object(d.a)({dangerouslySetInnerHTML:{__html:s}},c))}))})),st=Object(r.memo)((function(e){var t=e.messages,n=e.username,c=e.avatar,o=e.sendMessageAction,i=e.startSocketAction,s=rt(),u=Object(r.useState)(""),l=Object(g.a)(u,2),d=l[0],j=l[1],b=Object(r.useState)(null),f=Object(g.a)(b,2),O=f[0],p=f[1],m=Object(z.b)(),h=m.enqueueSnackbar,x=(m.closeSnackbar,Object(r.useState)("".concat(Date.now()))),v=Object(g.a)(x,2),y=v[0];v[1];Object(r.useEffect)((function(){i()}),[i]),Object(r.useEffect)((function(){O&&O.scrollHeight-O.scrollTop<1e3&&(O.scrollTop=O.scrollHeight)}),[t,O]);var w=function(e){var t=arguments.length>1&&void 0!==arguments[1]&&arguments[1];return{content:(e||"").toString(),isSelf:!0,time:Date.now(),isImage:t,name:n,avatar:c}},C=function(){o(w(d)),j("")},k=["ctrl + Enter \u4ee5\u8f93\u5165\u56de\u8f66","\u53ef\u4ee5\u76f4\u63a5\u53d1\u9001\u526a\u8d34\u677f\u4e2d\u7684\u56fe\u7247","+1 \u53ef\u4ee5\u590d\u8bfb","\u56fe\u7247\u5927\u5c0f\u5fc5\u987b\u5c0f\u4e8e5MB","\u6d88\u606f\u5386\u53f2\u6700\u591a\u53ea\u6709100\u6761"],S=function(e){var t=e.isSelf,n=e.name,r=e.time,c=e.isImage,o=e.content;return Object(a.jsxs)("div",{className:s.message,children:[Object(a.jsx)("div",{className:ke()(Object(ae.a)({},s.rightAlign,t)),children:"".concat(n," - ").concat(new Date(r).toLocaleTimeString("zh-CN",{hour12:!1}))}),Object(a.jsx)(Se.a,{className:ke()(Object(ae.a)({},s.myDivider,t))}),Object(a.jsx)("div",{className:s.messageContent,children:c?Object(a.jsx)(at,{src:o}):o.split("\n").map((function(e,t){return Object(a.jsxs)("span",{children:[e,Object(a.jsx)("br",{})]},t)}))})]})},E=function(e){var t=e.avatar,n=e.name;return Object(a.jsx)(We.a,{alt:n,src:t,className:s.avatar,children:Object(a.jsx)(it,{SvgProp:{style:{height:"36px",width:"36px"}},AvatarId:y})})};return Object(a.jsxs)($e.a,{className:s.messenger,children:[Object(a.jsx)("div",{className:s.messages,ref:p,children:t.map((function(e,t){return Object(a.jsxs)("div",{className:ke()(s.messageContainer,Object(ae.a)({},s.my,e.isSelf)),children:[E(e),Object(a.jsx)(Ke.a,{label:S(e),classes:{root:ke()(s.chipRoot,Object(ae.a)({},s.myChip,e.isSelf))}})]},t)}))}),Object(a.jsxs)("div",{className:s.input,children:[Object(a.jsx)(Se.a,{}),Object(a.jsxs)("div",{className:s.inputContent,children:[Object(a.jsx)("input",{accept:"image/png, image/jpeg",className:s.hidden,id:"file",type:"file",onChange:function(e){var t=e.target.files;if(t){var n=t[0],a=n.name.split(".").slice(-1)[0];if(["jpg","jpeg","png"].includes(a))if(n.size>5242880)h("\u56fe\u7247\u5927\u5c0f\u5fc5\u987b\u5c0f\u4e8e5MB",{variant:"info"});else{var r=new FileReader;r.readAsDataURL(n),r.onload=function(){o(w(r.result,!0))}}else h("\u8bf7\u4e0a\u4f20jpg\u6216png\u7c7b\u578b\u7684\u56fe\u7247",{variant:"info"})}else h("\u4f60\u6ca1\u6709\u4e0a\u4f20\u4efb\u4f55\u56fe\u7247",{variant:"info"})},onClick:function(e){e.currentTarget.value=""}}),Object(a.jsx)("label",{htmlFor:"file",children:Object(a.jsx)(se.a,{color:"primary",component:"span",children:Object(a.jsx)(Qe.a,{})})}),Object(a.jsx)(se.a,{color:"primary",component:"span",onClick:function(){var e=t[t.length-1];e&&o(w(e.content,e.isImage))},disabled:!t.length,children:Object(a.jsx)(Ye.a,{})}),Object(a.jsx)(qe.a,{multiline:!0,rowsMax:4,value:d,placeholder:k[~~(Math.random()*k.length)],className:s.textField,margin:"normal",onChange:function(e){var t=e.target.value;j(t)},onKeyPress:function(e){var t=e.ctrlKey,n=e.charCode;t&&13===n&&j((function(e){return e+"\n"})),t||13!==n||(e.preventDefault(),d&&d.match(/\S+/)&&C())},onPaste:function(e){for(var t=(e.clipboardData||e.originalEvent.clipboardData).items,n=null,a=0,r=Object.values(t);a<r.length;a++){var c=r[a];0===c.type.indexOf("image")&&(n=c.getAsFile())}if(null!==n){var i=new FileReader;i.onload=function(){o(w(i.result,!0))},i.readAsDataURL(n)}}}),Object(a.jsx)(se.a,{color:"primary",component:"span",onClick:C,disabled:!(d&&d.match(/\S+/)),children:Object(a.jsx)(et.a,{})})]})]})]})})),ut=Object(l.b)((function(e){var t=e.User,n=t.messages,a=t.info;return{messages:n,username:a.username,avatar:a.avatar}}),(function(e){return Object(u.bindActionCreators)({sendMessageAction:b.F,startSocketAction:b.J},e)}))(st),lt=Object(r.memo)((function(e){e.updateSudokuAction,e.playRoundBackwardAction;var t=Ue(),n=Object(r.useState)(!0),c=Object(g.a)(n,2),o=c[0],i=c[1],s=Object(r.useState)(!1),u=Object(g.a)(s,2),l=u[0],d=u[1],j=Object(r.useState)(null),b=Object(g.a)(j,2),f=b[0],O=b[1],p=Object(r.useState)(!1),m=Object(g.a)(p,2),h=m[0],x=m[1],v=Object(r.useState)(!1),y=Object(g.a)(v,2),w=y[0],C=y[1],k=Object(r.useState)(!1),S=Object(g.a)(k,2),E=S[0],T=S[1],B=Object(r.useContext)(J.a),A=(B.darkMode,B.setDarkMode),N=function(e){var t=e.currentTarget;O(t)},I=function(){x((function(e){return!e}))},L=function(){T((function(e){return!e}))},R=Object(r.useMemo)((function(){return Object(a.jsx)(re.a,{children:"\u8fd9\u91cc\u662f\u6211\u4eec\u8981\u505a\u7684\u4e1c\u897f"})}),[]);Object(je.a)({target:void 0});return Object(a.jsxs)(a.Fragment,{children:[Object(a.jsx)(ce.a,{appear:!1,direction:"down",in:o,children:Object(a.jsx)(oe.a,{position:"fixed",className:ke()(t.appBar,Object(ae.a)({},t.appBarShift,l)),children:Object(a.jsxs)(ie.a,{disableGutters:!l,classes:{gutters:t.appBarGutters,regular:t.regular},children:[Object(r.useMemo)((function(){return Object(a.jsx)(se.a,{color:"inherit",className:ke()(t.menuButton,{}),onClick:function(){C((function(e){return!e})),d((function(e){return!e}))},children:Object(a.jsx)(he.a,{})})}),[l,t.menuButton]),Object(r.useMemo)((function(){return Object(a.jsxs)("div",{className:ke()(t.rightButtons),children:[Object(a.jsx)(ze.a,{TooltipProp:{title:"Messenger",children:Object(a.jsx)(a.Fragment,{})},IconButtonProp:{onClick:L,color:"inherit"},children:Object(a.jsx)(we.a,{})}),Object(a.jsx)(ze.a,{TooltipProp:{title:"Help",children:Object(a.jsx)(a.Fragment,{})},IconButtonProp:{onClick:I,color:"inherit"},children:Object(a.jsx)(me.a,{})}),Object(a.jsx)(ze.a,{TooltipProp:{title:"Dark",children:Object(a.jsx)(a.Fragment,{})},IconButtonProp:{onClick:A,color:"inherit"},children:Object(a.jsx)(be.a,{})}),Object(a.jsx)(ze.a,{TooltipProp:{title:"Logout",children:Object(a.jsx)(a.Fragment,{})},IconButtonProp:{onClick:N,color:"inherit"},children:Object(a.jsx)(ve.a,{})})]})}),[l,t.rightButtons]),Object(a.jsx)(ue.a,{anchorEl:f,open:Boolean(f),onClose:function(){O(null)},children:Object(a.jsx)(le.a,{children:"\u9000\u51fa"})})]})})}),Object(a.jsx)(de.a,{in:E,classes:{container:t.collapse},children:Object(a.jsx)(ut,{})}),Object(a.jsx)(Fe,{title:"help",open:h,onClose:I,children:R}),Object(a.jsx)(_e,{open:w}),Object(a.jsxs)(ze.a,{TooltipProp:{title:o?"\u9690\u85cf":"\u663e\u793a",children:Object(a.jsx)(a.Fragment,{})},IconButtonProp:{className:ke()(t.hideButtonShowing,Object(ae.a)({},t.hideButton,!o)),onClick:function(){i((function(e){return!e}))}},children:[Object(a.jsx)(fe.a,{className:ke()(Object(ae.a)({},t.hide,!o))}),Object(a.jsx)(Oe.a,{className:ke()(Object(ae.a)({},t.hide,o))})]})]})})),dt=Object(l.b)(null,(function(e){return Object(u.bindActionCreators)({updateSudokuAction:b.O,playRoundBackwardAction:b.C},e)}))(lt),jt=Object(Z.a)((function(e){e.palette;var t=e.spacing,n=e.breakpoints;return Object(ee.a)({root:{display:"flex"},content:Object(ae.a)({flexGrow:1,marginTop:t(8),height:"calc(100vh - 64px)",overflowX:"auto"},n.down("sm"),{marginTop:t(6),height:"calc(100vh - 48px)"})})})),bt=Object(r.memo)((function(e){var t=e.children,n=e.userInfo,r=e.loading,c=jt();return Object(a.jsxs)("div",{className:c.root,children:[Object(a.jsx)(dt,{}),Object(a.jsx)("main",{className:c.content,children:n&&t}),r&&Object(a.jsx)(ne,{})]})})),ft=Object(l.b)((function(e){var t=e.User;return{userInfo:t.info,loading:t.loading}}),(function(e){return Object(u.bindActionCreators)({},e)}))(bt),Ot=Object(r.lazy)((function(){return Promise.all([n.e(3),n.e(4)]).then(n.bind(null,350))})),pt=Object(r.memo)((function(){var e,t=Object(r.memo)((function(){return Object(a.jsx)(U.a,{to:"/game"})}));return Object(a.jsx)(c.a.Fragment,{children:Object(a.jsx)(q,{children:Object(a.jsxs)(U.d,{children:[Object(a.jsx)(U.b,{path:"/game",render:(e=Object(a.jsx)(Ot,{}),function(t){return Object(a.jsx)(ft,Object(d.a)(Object(d.a)({},t),{},{children:Object(a.jsx)(r.Suspense,{fallback:Object(a.jsx)(ne,{}),children:e})}))})}),Object(a.jsx)(U.b,{path:"/",component:t})]})})})})),mt=Object(U.g)(Object(J.b)(pt)),gt=n(321),ht=n(323),xt=n(170),vt=n(320),yt=n(167),wt=n.n(yt),Ct=n(173),kt=n(169),St=n(324),Et=n(168),Tt="http://hustunique.com:5000",Bt=[function(e,t,n){return Object(j.a)(n),e.pipe(Object(Ct.a)(b.g),Object(Et.a)((function(){return St.a.getJSON("".concat(Tt,"/user/qrCode")).pipe(Object(Et.a)((function(e){if("success"===e.type)return Object(kt.a)(Object(b.G)(e.type,e.message));throw Error(e.message)})))})))}],At=n(16),Nt=n(317),It=n(325),Lt=n(318),Rt=[function(e,t,n){var a=n.io,r=n.socket$;return e.pipe(Object(Ct.a)(b.p),Object(Nt.a)((function(){return new At.a((function(e){var t=a(Tt);t.on("connect",(function(){return e.next(t)})),t.on("disconnect",t.close)}))})),Object(It.a)(r),Object(Lt.a)())},function(e,t,n){return n.socket$.pipe(Object(Nt.a)((function(e){return e?new At.a((function(t){e.on("message",(function(e){var n=e.message;t.next(Object(b.v)(Object(d.a)(Object(d.a)({},n),{},{isSelf:!1})))})),e.on("id",(function(e){var t=e.id;console.log(t)}))})):xt.a})))}],Mt=n(65),Pt=n(319),Dt=[function(e,t,n){return n.socket$.pipe(Object(Nt.a)((function(t){return t?e.pipe(Object(Ct.a)(b.l),Object(It.a)((function(e){var n=e.message;t.emit("message",{message:n}),console.log("emit message finish")})),Object(Mt.a)((function(e){var t=e.message;return Object(b.v)(t)})),Object(Pt.a)((function(e){return Ht(e)}))):xt.a})),Object(Pt.a)((function(e){return Ht(e)})))}],Ht=function(e,t){e.message,e.type;return xt.a},_t={io:wt.a,socket$:new vt.a(null),sessionStorage:sessionStorage},Gt=Object(gt.a)({dependencies:_t}),Vt=ht.a.apply(void 0,Object(O.a)(Bt).concat(Object(O.a)(Rt),Object(O.a)(Dt)));Object(s.a)();var Ft=[Gt],Ut=Object(u.createStore)(G,Object(F.composeWithDevTools)(u.applyMiddleware.apply(void 0,Ft)));Gt.run(Vt);var zt=Object(r.memo)((function(){return Object(a.jsx)(l.a,{store:Ut,children:Object(a.jsx)(V.a,{children:Object(a.jsx)(mt,{})})})})),Wt=function(e){e&&e instanceof Function&&n.e(5).then(n.bind(null,349)).then((function(t){var n=t.getCLS,a=t.getFID,r=t.getFCP,c=t.getLCP,o=t.getTTFB;n(e),a(e),r(e),c(e),o(e)}))};i.a.render(Object(a.jsx)(zt,{}),document.getElementById("root")),Wt()},71:function(e,t,n){"use strict";var a=n(7),r=n(2),c=n(0),o=n(257),i=n(255),s=Object(c.memo)((function(e){var t=e.TooltipProp,n=e.IconButtonProp,c=e.children;return Object(r.jsx)(o.a,Object(a.a)(Object(a.a)({},t),{},{children:Object(r.jsx)(i.a,Object(a.a)(Object(a.a)({},n),{},{children:c}))}))}));t.a=s},90:function(e,t,n){"use strict";n.d(t,"a",(function(){return f}));var a=n(7),r=n(17),c=n(2),o=n(0),i=n(122),s=n.n(i),u=n(253),l=n(252),d=n(124),j=n.n(d),b=n(251),f=Object(o.createContext)({darkMode:!1,setDarkMode:function(){return null}}),O=j()({palette:{type:"dark",primary:{main:"#343434",dark:"#000000",light:"#484848",contrastText:"#ffffff"},secondary:{main:"#ff7597"},background:{default:"#121212"},boardBackgroundColor:{main:"#000000",contrastText:"#ffffff"},optionNumberBackgroudColor:{main:"#000000",contrastText:"#ffffff"},numberBackgroundColor:{main:"rgba(0,0,0,0)",contrastText:"#ffffff"}},overrides:{MuiIconButton:{colorPrimary:{color:"white"}},MuiButton:{textPrimary:{color:"white"}},MuiCheckbox:{colorPrimary:{"&$checked":"inherit"}}}}),p=j()({palette:{primary:s.a,background:{default:"#f1f1f1"},boardBackgroundColor:{main:"#FF8000"},numberBackgroundColor:{main:"rgba(0,0,0,0)",contrastText:"#ffffff"},optionNumberBackgroudColor:{main:"rgba(0,0,0,0)",contrastText:"#ffffff"}}});t.b=function(e){return function(t){var n=Object(b.a)("(prefers-color-scheme: dark)"),i=Object(o.useState)(!1),s=Object(r.a)(i,2),d=s[0],j=s[1];return Object(o.useEffect)((function(){j(n)}),[n]),Object(c.jsx)(f.Provider,{value:{darkMode:d,setDarkMode:function(){return j(!d)}},children:Object(c.jsxs)(l.a,{theme:d?O:p,children:[Object(c.jsx)(u.a,{}),Object(c.jsx)(e,Object(a.a)({},t))]})})}}}},[[247,1,2]]]);
//# sourceMappingURL=main.f1757d00.chunk.js.map