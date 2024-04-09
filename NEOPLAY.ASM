csatorna equ 9
sor_szel equ 2*csatorna
pattern_szel equ 64*sor_szel

.286

verem segment word stack 'stack'
db 128 dup (0)
verem ends

code segment byte public
  assume cs:code,ds:code,ss:verem

public enable
public equal
public hangszer
public global_hangero
public sajat_hangero
public track
public melyik_pattern
public hol_a_patternben
public init_proc

init_proc proc far
          pusha
          push ds

          mov bx,dx
          mov cx,ds
          push cs
          pop ds

          cmp ax,0
          jne nem_reset
             call init_zax
             jmp loc_4
          nem_reset:
          cmp ax,1
          jne nem_start
             call Leallit_Zax
             mov ax,0
             call Elindit_Zax
             jmp loc_4
          nem_start:
          cmp ax,2
          jne nem_fade
             mov ax,65535
             call Elindit_Zax
             jmp loc_4
          nem_fade:
          cmp ax,3
          jne loc_4
             call Leallit_Zax

          loc_4:
          pop ds
          popa
          retf
init_proc endp

init_zax proc near
         mov ax,351ch
         int 21h
         mov save_1ch_seg,es
         mov save_1ch_ofs,bx
         call init_adlib
;         jnc jo_init
;           mov dx,offset adlib_e
;           call Kiiras
;         jo_init:
         retn
init_zax endp

init_irq:
      pusha
      push ds
      push es

      push cs
      pop ds
      push zene_szegmens
      pop es

      dec hol_tart_a_delay
      cmp hol_tart_a_delay,0
      jne loc_5
         mov al,delay
         mov hol_tart_a_delay,al
         call tovabb_zene
      loc_5:
      call out_szolam
      call set_hangero
      call set_equal

;int 60h

      pop es
      pop ds
      popa
      iret

set_hangero proc near
            cmp csokkentes,0
            je nem_csokkentes
              cmp global_hangero,63
              je nem_csokkentes
                ;!!! 63-as global_hangero a leghalkabb !!!
                inc global_hangero
            nem_csokkentes:
            cmp noveles,0
            je nem_noveles
              cmp global_hangero,0
              je nem_noveles
                 dec global_hangero

            nem_noveles:
            retn
set_hangero endp

tovabb_zene proc near
            mov si,zene_offset
            add si,633h
            mov bx,zene_offset
            add bx,600h
            mov al,melyik_pattern
            cbw
            add bx,ax
            xor ax,ax

            mov al,es:[bx]       ;SI-be a pattern offsetc¡me
            mov track,al         ;
            mov bx,pattern_szel  ;64*18 -> 64 sor,9 csatorna 2 byte-tal)
            mul bx               ;
            add si,ax            ;

            mov al,hol_a_patternben  ;Kisz molja a megfelel“ pattern
            cbw                      ;megfelel“ sor nak offsetc¡m‚t
            mov bl,sor_szel          ;
            mul bl                   ;
            add si,ax                ;

            mov cx,9
            locloop_8:
               call decode_codes
            loop locloop_8

            inc hol_a_patternben
            cmp hol_a_patternben,64
            jne nincs_pattern_veg
               mov hol_a_patternben,0 ;k”vetkez“ pattern
               inc melyik_pattern     ;

               mov si,zene_offset
               add si,600h
               xor bx,bx
               mov bl,melyik_pattern
               mov al,es:[bx+si]      ;AL-be k”vetkez“ pattern sorsz ma

               cmp al,255             ;Dalv‚g?
               jne nincs_pattern_veg  ;
                  mov melyik_pattern,0 ;Na akkor kezdjk £jra

            nincs_pattern_veg:
            retn
tovabb_zene endp

Set_Frekvencia_Offset proc near
                      mov di,offset szolam_frekvencia
                      mov bx,9
                      sub bx,cx
                      shl bx,1
                      inc ah
                      retn
Set_Frekvencia_offset endp

Set_Hangszer_Offset proc near
                    push ax
                    push bx
                    mov al,hangszer[bx]
                    cbw
                    mov bx,12
                    mul bx
                    mov si,zene_offset
                    add si,ax
                    pop bx
                    pop ax
                    retn
Set_Hangszer_offset endp

Set_Teltseg proc near
            shl ah,2
            mov bx,9
            sub bx,cx
            mov [bx+di],ah
            retn
Set_Teltseg endp

CommandCodes proc near
             mov ah,al    ;pakolt byte -> fels“ n‚gy bit vez‚rl¢k¢d
             and ax,0FF0H ;                als¢ n‚gy bit param‚ter
             shr al,4     ;Vagyis pl: A3 -> vez‚rl“k¢d: 0ah
                          ;              ->  param‚ter: 03h

             cmp al,1
             jne nem_novel_freki
                call Set_Frekvencia_Offset
                add [bx+di],ah

             nem_novel_freki:
             cmp al,2
             jne nem_csokkent_freki
                call Set_Frekvencia_Offset
                sub [bx+di],ah

             nem_csokkent_freki:
             cmp al,5
             jne nem_set_ritmus
                ;Na ez egy ‚rdekes r‚sz,‚n m‚g semmi okosat nem tudtam
                ;”sszehozni bel“le.Ahogy ¡gy kiszedtem a forr sb¢l,a kurrens
                ;hangform n  ll¡t be £j "ritmust".Ha a param‚ter
                ;1 ,akkor Hi Hat
                ;2        Cymbal
                ;3        Tom-tom
                ;4        Snare
                ;5        Bass
                ;
                ;Mondom,hogy csak szerintem,mivel kipr¢b lni m‚g nem tudtam.

                push cx
                push bx
                mov bl,1
                mov cl,ah
                shl bl,cl
                and hangforma,0E0H ;Als¢ 5 bitet t”rli
                and bl,31          ;fels“ 5 bitet t”rli
                or hangforma,bl    ;be ll¡tja a megfelel“ ritmust
                pop bx
                pop cx

             nem_set_ritmus:
             cmp al,6
             jne nem_feedback
                pusha
                and ah,7 ;T”rli a fels“ 5 bitet
                shl ah,1 ;Eggyel balra visszk a feedbacket
                mov bx,9
                sub bx,cx

                call Set_Hangszer_Offset

                mov al,es:[si+8]
                and al,1
                or al,ah

                push ax
                mov dx,AdLib_port1
                mov al,0C0H
                mov bx,9
                sub bx,cx
                add al,bl
                out dx,al
                call sub_13a
                mov dx,AdLib_port2
                pop ax
                out dx,al
                call sub_13b
                popa

             nem_feedback:
             cmp al,0AH
             jne nem_teltseg
                ;Ez a hangnak a "kontrasztoss g t","telts‚g‚t"  ll¡tja be
                ;00H -> "vastag"
                ;0FH -> "veghang"
                mov di,offset teltseg
                call Set_Teltseg

             nem_teltseg:
             cmp al,0BH
             jne nem_elesseg
                ;Ez a hangnak az ‚less‚g‚t  ll¡tja be
                ;00H -> les a hang
                ;0FH -> Tompa

                mov di,offset elesseg
                call set_teltseg

             nem_elesseg:
             cmp al,0CH
             jne nem_szolam_hangero
                ;Az adott sz¢lam hangerej‚t  ll¡tja be
                ;00H -> Leghangosabb
                ;0FH -> Csend

                mov di,offset teltseg
                call Set_Teltseg

                push si
                call Set_Hangszer_Offset

                test byte ptr es:[si+8],1

                pop si
                jz nincs_feedback
                  mov elesseg[bx],ah
                nincs_feedback:

             nem_szolam_hangero:
             cmp al,0DH
             jne nem_pattern_eleje
                ;Ugr s a param‚terben megadott pattern elej‚re
                mov melyik_pattern,ah
                mov hol_a_patternben,63

             nem_pattern_eleje:
             cmp al,0FH
             jne return_master
                ;Sebess‚g param‚terben megadottra
                inc ah
                mov delay,ah
                mov hol_tart_a_delay,ah
             return_master:
             retn
CommandCodes endp

decode_codes proc near
             xor ax,ax
             mov al,es:[si]
             inc si

             cmp al,80H
             jne nem_hangszervalasztas
               mov al,es:[si]         ;AL-be be ll¡tand¢ hangszerk¢dja
               and al,127             ;Fels“ bitet t”r”ljk /csak max.
               inc si                 ;128 hangszer lehet/
               mov ah,9
               sub ah,cl
               call beallit_hangszer
               jmp loc_29

             nem_hangszervalasztas:
             cmp al,0
             je nem_hangkiadas
                mov ah,9       ;Kiad egy AL-nek megfelel“ k¢d£
                sub ah,cl      ;frekvenci t a sz¢lamra
                call kiad_hang ;

             nem_hangkiadas:
             mov al,es:[si]  ;"commmand" byte AL-be
             inc si
             cmp al,0
             jne valami_tortent
               jmp loc_29

             valami_tortent:
             cmp al,1
             jne nem_restart_pattern
               mov hol_a_patternben,63

             nem_restart_pattern:
             cmp al,2
             jne nem_halkitas

               mov csokkentes,0FFh ;Lehalk¡tja a zen‚t
               mov noveles,0       ;

             nem_halkitas:
             cmp al,3
             jne nem_hangositas
               mov noveles,0FFh      ;Feler“s¡ti a zen‚t
               mov csokkentes,0      ;
	       mov global_hangero,63 ;

             nem_hangositas:
             cmp al,4
             jne nem_maxi_hangero
               mov global_hangero,0  ;Glob lis hanger“ csutk ra

             nem_maxi_hangero:
             cmp al,5
             jne nem_ritmus       ;A hangforma 5.bitj‚t be ll¡tja,ami
               or hangforma,32    ;ha be van  ll¡tva,akkor 6 dallamos
                                  ;hangcsatorn t enged‚lyez
                                  ;"ritmus be"
             nem_ritmus:
             cmp al,6
             jne nem_dallam      ;Ez meg t”rli az omin¢zus 5. bitet,
               and hangforma,223 ;vagyis 9 dallamos hangcsatorna van
                                 ;enged‚lyezve "ritmus ki"

             nem_dallam:
             call CommandCodes

             loc_29:
             mov al,hangforma
             cmp hangforma_ment,al
             je ugyanaz_a_hangforma
               mov hangforma_ment,al

               mov port1_be,0BDH
               mov al,hangforma
               mov port2_be,al
               call Set_Adlib

             ugyanaz_a_hangforma:
             retn
decode_codes endp

Leallit_Zax proc near
            pusha
            push ds

            push cs
            pop ds
            mov ax,251ch
            mov dx,save_1ch_ofs
            mov cx,save_1ch_seg
            mov ds,cx
            int 21h

            push cs
            pop ds
            call kikapcsol_szolamok

            pop ds
            popa
            retn
Leallit_Zax endp

Elindit_Zax proc near
            pusha
            push es
            push ds

            push cs
            pop ds
            cmp al,0FFh
            jne nem_halkit
              mov csokkentes,al
              mov noveles,0
              jmp loc_34

            nem_halkit:
            mov zene_offset,bx
            mov zene_szegmens,cx
            call kikapcsol_szolamok

            mov port1_be,1
            mov port2_be,32
            call Set_Adlib

            mov port1_be,8
            mov port2_be,64
            call Set_Adlib

            mov hangforma,0
            mov port1_be,0BDH
            mov al,0
            mov port2_be,al
            call Set_Adlib

            xor ax,ax
            mov cx,9

            push es

            push zene_szegmens
            pop es
            locloop_33:
              call beallit_hangszer
              add ax,0101h
            loop locloop_33
            pop es

            mov global_hangero,0
            mov csokkentes,0
            mov noveles,0
            mov hol_tart_a_delay,1
            mov melyik_pattern,0
            mov hol_a_patternben,0
            mov delay,2

            push cs
            pop ds
            mov ax,251ch
            mov dx,offset init_irq
            int 21h

            loc_34:
            pop ds
            pop es
            popa
            retn
Elindit_Zax endp

kikapcsol_szolamok proc near
                   mov port1_be,1
                   mov port2_be,0
                   call Set_Adlib

                   mov cx,0
                   ciklus1:
                   push cx

                     add cx,0b0h
                     mov port1_be,cl
                     mov port2_be,0
                     call Set_Adlib

                   pop cx
                   inc cx
                   cmp cx,8
                   jbe ciklus1

                   mov cx,0
                   ciklus2:
                   push cx

                     add cx,080h
                     mov port1_be,cl
                     mov port2_be,255
                     call Set_Adlib

                   pop cx
                   inc cx
                   cmp cx,5
                   jbe ciklus2

                   mov cx,8
                   ciklus3:
                   push cx

                     add cx,080h
                     mov port1_be,cl
                     mov port2_be,255
                     call Set_Adlib

                   pop cx
                   inc cx
                   cmp cx,0dh
                   jbe ciklus3

                   mov cx,0
                   ciklus4:
                   push cx

                      add cx,090h
                      mov port1_be,cl
                      mov port2_be,255
                      call Set_Adlib

                   pop cx
                   inc cx
                   cmp cx,5
                   jbe ciklus4

                   mov di,offset szolam_frekvencia
                   xor ax,ax
                   push es
                   push ds
                   pop es
                   mov cx,9
                   rep stosw
                   pop es
                   retn
kikapcsol_szolamok endp

beallit_hangszer proc near
                 pusha

                 xor cx,cx
                 xor bx,bx
                 mov bl,ah
                 mov hangszer[bx],al
                 mov cl,ah

                 cbw
                 mov bx,12
                 mul bx
                 mov si,zene_offset
                 add si,ax


                 mov al,0B0H
                 add al,cl
                 mov port1_be,al
                 mov port2_be,0
                 call Set_Adlib

                 mov al,0C0H
                 add al,cl
                 mov port1_be,al
                 mov al,es:[si+8]
                 mov port2_be,al
                 call Set_Adlib

                 mov di,offset decode_ports
                 mov bx,cx
                 rol bx,1
                 add di,bx

                 mov bx,0
                 set_ciklus:
                 push bx

                 mov al,tabla[bx]
                 add al,[di]
                 mov port1_be,al
                 mov al,es:[si+bx]
                 mov port2_be,al
                 call Set_Adlib

                 mov al,tabla[bx]
                 add al,[di+1]
                 mov port1_be,al
                 mov al,es:[si+bx+1]
                 mov port2_be,al
                 call Set_Adlib

                 pop bx
                 add bx,2
                 cmp bx,8
                 jb set_ciklus

                 mov al,0E0H
                 add al,[di]
                 mov port1_be,al
                 mov al,es:[si+9]
                 mov port2_be,al
                 call Set_Adlib

                 mov al,0E0H
                 add al,[di+1]
                 mov port1_be,al
                 mov al,es:[si+10]
                 mov port2_be,al
                 call Set_Adlib

                 mov bx,cx
                 mov al,es:[si+2]
                 mov teltseg[bx],al
                 mov al,es:[si+3]
                 mov elesseg[bx],al

                 mov al,es:[si+11]
                 and al,240
                 shr al,4
                 mov frekvencia_hi[bx],al

                 popa
                 retn
beallit_hangszer endp

kiad_hang proc near
          pusha

          cmp al,7FH
          jne nem_csend
            mov bl,ah
            xor bh,bh
            rol bx,1
            mov ax,szolam_frekvencia[bx]
            and ah,223 ;5. bitet t”r”lni
            mov szolam_frekvencia[bx],ax
            jmp short loc_36
          nem_csend:

          push bx
          xor bx,bx
          mov bl,ah
          mov kell_equal[bx],1
          pop bx

          dec al
          mov kurrens_hang,al
          xor cx,cx
          mov cl,ah

          mov al,0B0H
          add al,cl
          mov port1_be,al
          mov port2_be,0
          call Set_Adlib

          xor bx,bx
          mov bl,kurrens_hang
          rol bx,1
          mov ax,okt[bx]

          mov bx,cx
          rol bx,1
          mov szolam_frekvencia[bx],ax

          loc_36:
          popa
          retn
kiad_hang endp

out_szolam proc near
           pusha
           mov cx,9

           locloop_38:
           dec cx
              mov bx,cx
              cmp enable[bx],0
              je nincs_csatorna
                call set_csatorna
              nincs_csatorna:
           inc cx
           loop locloop_38

           popa
           retn
out_szolam endp

set_global_hangero proc near
                   push dx
                   push bx

                   mov dh,al
                   and dh,192
                   and ax,63
                   push dx

                   mov bl,63
                   sub bl,global_hangero
                   xor bh,bh
                   mul bx
                   mov bx,63
                   div bx

                   mov bl,sajat_hangero
                   xor bh,bh
                   mul bx
                   mov bx,100
                   div bx

                   pop dx
                   mov dl,al
                   mov al,63
                   sub al,dl
                   and al,63
                   or al,dh

                   pop bx
                   pop dx
                   retn
set_global_hangero endp

set_csatorna proc near
             mov al,0A0H
             add al,cl
             mov port1_be,al

             mov si,cx
             rol si,1
             mov bx,cx

             mov al,byte ptr szolam_frekvencia[si]
             add al,frekvencia_hi[bx]
             mov port2_be,al
             call Set_Adlib

             mov al,0B0H
             add al,cl
             mov port1_be,al
             mov al,byte ptr szolam_frekvencia[si+1]
             mov port2_be,al
             call Set_Adlib

             mov al,40H
             add al,decode_ports[si]
             mov port1_be,al
             mov al,63
             sub al,teltseg[bx]

             call Set_Global_Hangero

             mov port2_be,al
             Call Set_Adlib

             mov al,40h
             add al,decode_ports[si+1]
             mov port1_be,al
             mov al,elesseg[bx]
             pusha

             push bx
             mov bx,cx
             call Set_Hangszer_Offset
             pop bx

             test byte ptr es:[si+8],1
             popa
             jz loc_40
                mov al,63
                sub al,elesseg[bx]
                call Set_Global_Hangero

             loc_40:
             mov port2_be,al
             call Set_Adlib
             retn
set_csatorna endp

set_equal proc near
          pusha
          mov cx,9
          mov di,8
          beallit_equal:

          cmp kell_equal[di],0
          je nincs_equalizer

              mov ax,szolam_frekvencia[di]
              and ah,63
              mov al,128
              sub al,ah
              shr al,3
              cmp al,15
              jbe kisebb_15
                 mov al,15
              kisebb_15:
              inc al
              mov equal[di],al
              mov kell_equal[di],0


          nincs_equalizer:
          cmp equal[di],0FFh
          je lecsenges_vege
            dec equal[di]

          lecsenges_vege:
          dec di
          loop beallit_equal

          popa
          retn
set_equal endp

init_adlib proc near
           pusha
           push ds

           push cs
           pop ds

           mov si,0
           check_ports:
           push si

           mov ax,ports[si]
           mov adlib_port1,ax
           inc ax
           mov adlib_port2,ax

           mov port1_be,4
           mov port2_be,60h
           call set_adlib
           mov port1_be,4
           mov port2_be,80h
           call set_adlib

           mov dx,Adlib_port1
           in al,dx
           and al,0e0h
           mov ment_ax_1,al

           mov port1_be,2
           mov port2_be,0ffh
           call set_adlib
           mov port1_be,4
           mov port2_be,21h
           call set_adlib

           mov dx,3dah
           vi1:
            in al,dx
            test al,8
           jne vi1
           vi2:
            in al,dx
            test al,8
           je vi2

           mov dx,Adlib_port1
           in al,dx
           and al,0e0h
           mov ment_ax_2,al

           mov port1_be,4
           mov port2_be,60h
           call set_adlib
           mov port1_be,4
           mov port2_be,80h
           call set_adlib

           pop si
           clc
           xor ax,ax
           cmp ment_ax_1,0
           jne nincs_adlib
           cmp ment_ax_2,0c0h
           jne nincs_adlib
   vissza_init:
           pop ds
           popa
           retn


           nincs_adlib:
           cmp si,6
           jb lehet_meg_ports
             stc
             mov ax,65535
             jmp vissza_init
           lehet_meg_ports:
           add si,2
           jmp check_ports
init_adlib endp

set_adlib proc near
          mov dx,AdLib_port1
          mov al,port1_be
          out dx,al
          call sub_13a
          mov dx,AdLib_port2
          mov al,port2_be
          out dx,al
          call sub_13b
          retn
set_adlib endp

sub_13a proc near
        push cx
        mov cx,6
        read_port1:
          in al,dx
        loop read_port1
        pop cx
        retn
sub_13a endp

sub_13b proc near
        push cx
        mov cx,35
        read_port2:
          in al,dx
        loop read_port2
        pop cx
        retn
sub_13b endp

save_1ch_ofs dw 0
save_1ch_seg dw 0
AdLib_port1 dw 0
AdLib_port2 dw 0
ment_ax_1 db 0
ment_ax_2 db 0
port1_be db 0
port2_be db 0


ports dw 218h,288h,318h,388h
tabla db 32,0,64,0,96,0,128,0

okt dw  8555, 8577, 8600, 8624, 8650, 8677, 8706, 8736, 8769, 8803, 8839, 8878
    dw  9579, 9601, 9624, 9648, 9674, 9701, 9730, 9760, 9793, 9827, 9863, 9902
    dw 10603,10625,10648,10672,10698,10725,10754,10784,10817,10851,10887,10926
    dw 11627,11649,11672,11696,11722,11749,11778,11808,11841,11875,11911,11950
    dw 12651,12673,12696,12720,12746,12773,12802,12832,12865,12899,12935,12974
    dw 13675,13697,13720,13744,13770,13797,13826,13856,13889,13923,13959,13998
    dw 14699,14721,14744,14768,14794,14821,14850,14880,14913,14947,14983,15022
    dw 15723,15745,15768,15792,15818,15845,15874,15904,15937,15971,16007,16046
    dw     0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0
    dw     0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0

decode_ports db  3, 0
             db  4, 1
             db  5, 2
             db 11, 8
             db 12, 9
             db 13,10
             db 19,16
             db 20,17
             db 21,18
             db 36 dup (0)

kell_equal db 9 dup (0)
szolam_frekvencia dw 9 dup (0)
teltseg db 9 dup (0)
elesseg db 9 dup (0)
frekvencia_hi db 9 dup (0)

zene_offset dw 0
zene_szegmens dw 0

delay db 0
hol_tart_a_delay db 0

kurrens_hang db 0

csokkentes db 0
noveles db 0

hangforma db 0
hangforma_ment db 0

enable db 9 dup (1)
equal db 9 dup (0)
hangszer db 0,1,2,3,4,5,6,7,8
global_hangero db 0
sajat_hangero db 100
track db 0
melyik_pattern db 0
hol_a_patternben db 0


Kiiras proc near
       push cs
       pop ds
       mov ah,9
       int 21h
       retn
Kiiras endp

GetMem proc near
       mov ah,48h
       int 21h
       retn
GetMem endp

FreeMem proc near
        mov ah,49h
        int 21h
        retn
FreeMem endp

AllocateMem proc near
            mov ah,4Ah
            int 21h
            retn
AllocateMem endp

LefoglalMem proc near
            push es
            pusha

            mov ax,cs:psp
            mov es,ax
            mov bx,es:[2]
            mov ax,es
            sub bx,ax
            sub bx,cs:mem_kell
            dec bx

            mov cs:mem_err,0
            call AllocateMem
            jnb tovabb_megy
              mov cs:mem_err,1
            tovabb_megy:
            mov bx,cs:mem_kell
            call GetMem
            jnc tovabb_megy2
              mov cs:mem_err,1
            tovabb_megy2:
            mov cs:zene_szeg,ax

            popa
            pop es
            retn
LefoglalMem endp

ReadFile proc near
         push cs
         pop ds
         mov ax,3d00h
         mov dx,offset file_name
         int 21h
         jnc jo_file
           mov dx,offset file_e
           call Kiiras
           jmp kilepes

         jo_file:
         mov cs:rfile,ax

         mov ax,4202h
         mov bx,cs:rfile
         xor cx,cx
         xor dx,dx
         int 21h
         pusha
         mov cs:zene_hossz,ax
         mov cl,4
         shr ax,cl
         inc ax
         mov cs:mem_kell,ax
         popa
         mov ax,4200h
         int 21h

         call LefoglalMem

         cmp cs:mem_err,0
         jne mem_err1
            mov ax,cs:zene_szeg
            mov ds,ax
            mov ah,3fh
            mov cx,cs:zene_hossz
            mov bx,cs:rfile
            mov dx,0
            int 21h

         mem_err1:
         mov bx,cs:rfile
         mov ah,3Eh
         int 21h
         retn
ReadFile endp

FileNev proc near
        push es
        pusha

        mov ax,cs:psp
        mov ds,ax

        cmp byte ptr ds:[80h],0
        jne van_file
          param_error:
          mov dx,offset file_e2
          call Kiiras
          jmp kilepes

        van_file:
        xor ch,ch
        mov cl,ds:[80h]
        mov si,80h
        @get_cikl:
         inc si
         cmp byte ptr ds:[si],20h
         jne vege_whitespace
        loop @get_cikl
        jmp param_error

        vege_whitespace:
        mov di,0
        bemasol:
          mov al,ds:[si]
          cmp al,20h
          jbe vege_file_name

        mov cs:file_name[di],al
        inc si
        inc di
        jmp bemasol

        vege_file_name:
        popa
        pop es
        retn
FileNev endp

Save_2fh proc near
         mov ax,352fh
         int 21h
         mov cs:orig_2fh_s,es
         mov cs:orig_2fh_o,bx
         retn
Save_2fh endp

Set_2fh proc near
        mov ax,252fh
        push cs
        pop ds
        mov dx,offset int_2fh_entry
        int 21h
        retn
Set_2fh endp

Restore_2fh proc near
            mov ax,252fh
            mov ds,cs:orig_2fh_s
            mov dx,cs:orig_2fh_o
            int 21h
            retn
Restore_2fh endp

int_2Fh_entry proc far
              cmp ax,0BBBBh
              jne nem_exit
                mov ax,3
                call init_proc   ;le ll¡tjuk a zen‚t

                mov es,cs:zene_szeg  ;free zax
                call FreeMem

                mov es,cs:psp    ;free TSR
                call FreeMem

                call Restore_2fh
                mov dx,offset remove
                call kiiras
                mov ax,0CCCCh

              nem_exit:
              jmp dword ptr cs:orig_2fh_o
int_2fh_entry endp


start: mov ah,30h
       int 21h
       mov bh,ah
       mov ah,al
       mov al,bh

       cmp ax,0300h
       jae jo_dos
         mov dx,offset dos_e
         call kiiras
         jmp kilepes

       jo_dos:
       mov ah,62h
       int 21h
       mov cs:psp,bx

       mov dx,offset set_text
       call Kiiras

       mov ax,0bbbbh
       int 2fh
       cmp ax,0cccch
       je kilepes
          call FileNev
          call ReadFile
          cmp cs:mem_err,1
          jne nincs_mem
             mov dx,offset memory_e
             call Kiiras
             jmp kilepes
          nincs_mem:
          call Save_2fh
          call Set_2fh

          mov ax,0       ;inicializ ljuk az Adlib t
          call init_proc ;

          mov ax,cs:zene_szeg
          mov ds,ax      ;ax=1 -> Elind¡tjuk a DS:DX-en l‚v“ zen‚t
          mov dx,0       ;
          mov ax,1       ;
          call init_proc

          mov dx,offset reroute
          call Kiiras

          mov es,cs:psp
          mov ax,es:[2ch]  ;Free envir.
          mov es,ax
          call FreeMem

          jmp tsr_exit


       kilepes:
       mov ax,4c00h
       int 21h

       TSR_exit:
       mov ax,3100h
       mov dx,256 ;256*16 byte lefoglalva
       int 21h

set_text db 'ECR',27h,'s repaired *NEO* player by The NutCracker/TWT'
         db 0dh,0ah,'$'
reroute db 'Timer interrupt rerouted...',0dh,0ah,'$'
remove db 'Player removed from memory ...',0dh,0ah,'$'
memory_e db 'Not enough memory!',0dh,0ah,'$'
file_e db 'File not found!',0dh,0ah,'$'
file_e2 db 'Try songname as paramater :)',0dh,0ah,'$'
dos_e db 'DOS 3.0+ required!',0dh,0ah,'$'
adlib_e db 'No AdLib compatible card present!',0dh,0ah,'$'


orig_2fh_o dw 0
orig_2fh_s dw 0

file_name db 15 dup (0)
psp dw 0
rfile dw 0
zene_hossz dw 0
zene_szeg dw 0
mem_kell dw 0
mem_err db 0

code ends
end start