#include "stdio.h"
#define NBLOCS 32

__attribute__((constructor)) void main()
{
    char c;
    char            buf[128*128];
    int             x;
    int             base = 0;
    char	    byte;



    tty_puts("h : main_horloge \nd : main_display\n");
    tty_getc_irq(&c);

    if(c=='d'){
        //main_display
        while(base < 21*NBLOCS)
            {
                tty_printf("\n *** starting image %d *** at date = %d \n", base/NBLOCS, proctime());

                /* Phase 1 : lecture image sur le disque */
                x = ioc_read(base, buf, NBLOCS);
                if( x )
                {
                        tty_printf("echec ioc_read = %d\n", x);
                        exit();
                }
                x = ioc_completed();
                if( x )
                {
                        tty_printf("echec ioc_completed = %d\n", x);
                        exit();
                }

                tty_printf("\n *** completing load  at date = %d \n", proctime());

                /* Phase 2 : affichage image */
                x = fb_sync_write(0, buf, 128*128);
                if( x )
                {
                        tty_printf("echec fb_sync_write\n");
                        exit();
                }
                base = base + NBLOCS;

                tty_printf("\n *** completing display at date = %d\n\n ... new image ?\n", proctime());
                tty_getc_irq(&byte);
            }
        goto end;
    }


    //main_horloge
    while(1){
     tty_puts("a : activation des interruptions générées par le TIMER \nd : desactivation des interruptions générées par le TIMER \nq : sortie de l'interprêteur par l'appel système exit() \n");
     tty_getc_irq(&c);

     switch(c){
        case 'a':
         timer_set_period(5000000);
         timer_set_mode(3);

         break;
        case 'd':
         timer_set_mode(0);
         break;
        case 'q':
         goto end;
         break;
     }
    }
    end:
    exit();
} // end main
