
grammar Slide;

prog: stat* EOF
    ;

stat: expr ';'
    ;


expr: slide
    ;

slide    : 'Slide' ID '{' (text | img | bg)* '}';
text    : 'Text'  '{' ((transform | text_prop) ';')* '}';
img     : 'Image' '{' ((transform | src) ';')* '}';


transform   : position
            | width
            | height
            ;

text_prop   : textCont
            | fontSize
            | fontFam
            | align
            | textColor
            ;

src         : 'src=' STR;
bg          : 'bg=' (COLOR | STR) ';';

position    : 'position=' UNIDAD ',' UNIDAD;
rotation    : 'rotation=' INT;
width       : 'width='  UNIDAD;
height      : 'height='  UNIDAD;

textCont    : 'content=' (STR | TEXT_BLOCK);
fontSize    : 'font-size=' INT;
fontFam     : 'font-family=' STR;
align       : 'align=' ALIGN;
textColor   : 'color=' COLOR;

ALIGN: 'center'
    |  'left'
    |  'right'
    |  'justified';

UNIDAD: INT+ ('%' | 'px') ;

INT : [0-9]+ ;
ID: [a-zA-Z_][a-zA-Z_0-9]* ;
COLOR: '#'[a-fA-F0-9]*;
STR : '"' (~[\r\n])* '"' ;
TEXT_BLOCK : '"""' .*? '"""';
LINE_COMMENT: '//' ~[\r\n]* -> skip;
BLOCK_COMMENT: '/*' .*? '*/' -> skip;

WS  : [ \t\r\n]+ -> skip;
