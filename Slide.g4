grammar Slide;

prog: stat* EOF
    ;

stat: expr
    ;


expr: slide
    ;

slide    : 'slide' '{' (text | img | background)* '}';
text    : 'text'  '{' ((transform | text_prop) ';')* '}';
img     : 'image' '{' ((transform | src | filter) ';')* '}';
background : ('background' | 'bg') '{' (bg ';')* '}';


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

blur     : 'blur(' UNIDAD ')';
brightness : 'brightness(' UNIDAD ')';
contrast  : 'contrast(' UNIDAD ')';
grayscale : 'grayscale(' UNIDAD ')';
hue_rotate : 'hue-rotate(' (INT | FLOAT) 'deg)';
invert    : 'invert(' UNIDAD ')';
saturate  : 'saturate(' UNIDAD ')';
opacity   : 'opacity(' UNIDAD ')';

filter   : 'filter=' (blur | brightness | contrast | grayscale | hue_rotate | invert | saturate | opacity)*;

src         : 'src=' STR;

position    : 'position=' UNIDAD ',' UNIDAD;
rotation    : 'rotation=' NUMBER;
width       : 'width='  UNIDAD;
height      : 'height='  UNIDAD;

textCont    : 'content=' (STR | TEXT_BLOCK);
fontSize    : 'font-size=' INT;
fontFam     : 'font-family=' STR;
align       : 'align=' ALIGN;
textColor   : 'color=' HEX;
bg          : 'bg=' (HEX | STR);

ALIGN: 'center'
    |  'left'
    |  'right'
    |  'justified';

UNIDAD: PERCENTAGE | PIXEL ;
PERCENTAGE: NUMBER '%';
PIXEL: INT 'px';

INT : [0-9]+ ;
FLOAT : [0-9]'.'[0-9]*;
NUMBER : (INT | FLOAT);
HEX: '#'[a-fA-F0-9]*;
STR : '"' (~[\r\n])* '"' ;
TEXT_BLOCK : '"""' .*? '"""';
LINE_COMMENT: '//' ~[\r\n]* -> skip;
BLOCK_COMMENT: '/*' .*? '*/' -> skip;

WS  : [ \t\r\n]+ -> skip;