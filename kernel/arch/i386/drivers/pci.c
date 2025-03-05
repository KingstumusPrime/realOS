#include <kernel/drivers/pci.h>
#include <kernel/inline.h>

pci_device_t* pci_dev_root = NULL;

void pci_add_device(unsigned char bus, unsigned char device, unsigned char function){
    pci_device_t* dev = malloc(sizeof(pci_device_t)); 

    dev = malloc(sizeof(pci_device_t));
    dev->bus = bus;
    dev->device = device;
    dev->function = function;

    dev->baseclass = get_base_class(bus, device, function);
    dev->subclass = get_sub_class(bus, device,function);
    dev->lf = get_prog_lf(bus, device, function);
    
    if(pci_dev_root == NULL){
        pci_dev_root = dev;
        pci_dev_root->next = NULL; 
    }else{
        dev->next = pci_dev_root->next;
        pci_dev_root->next = dev;
    }

}

unsigned short pci_config_read_word(unsigned char bus, unsigned char slot, unsigned char func, unsigned char offset){
    unsigned int address;
    unsigned int lbus = (unsigned int)bus;
    unsigned int lslot = (unsigned int) slot;
    unsigned int lfunc = (unsigned int)func;
    short tmp = 0;

    // create config address
    address = (unsigned int)((lbus << 16) | (lslot << 11)) |
                (lfunc << 8) | (offset & 0xFC) | ((unsigned int) 0x80000000);

    //send the address
    outl(CONFIG_ADDRESS, address);

    // read the data
    tmp = (unsigned short)((inl(CONFIG_DATA) >> ((offset & 2) * 8)) & 0xFFFF);

    return tmp;
}

void pci_config_write_word(unsigned char bus, unsigned char slot, unsigned char func, unsigned char offset, unsigned short data){
    unsigned int address;
    unsigned int lbus = (unsigned int)bus;
    unsigned int lslot = (unsigned int) slot;
    unsigned int lfunc = (unsigned int)func;
    short tmp = 0;

    // create config address
    address = (unsigned int)((lbus << 16) | (lslot << 11)) |
                (lfunc << 8) | (offset & 0xFC) | ((unsigned int) 0x80000000);

    //send the address
    outl(CONFIG_ADDRESS, address);

    // write the data
    outsw(CONFIG_DATA, data);

    return tmp;
}

void check_function(unsigned char bus, unsigned char device, unsigned char function ){
    unsigned short base_class;
    unsigned short sub_class;
    unsigned short secondary_bus;

    base_class = get_base_class(bus, device, function);
    sub_class = get_sub_class(bus, device, function);
    if((base_class == 0x6) && (sub_class == 0x4)){
        secondary_bus = get_secondary_bus(bus, device, function);
        check_bus(secondary_bus);
    }
}

void check_device(unsigned char bus, unsigned char device){
    unsigned short function = 0;

    unsigned short vendor = get_vendor_id(bus, device, function);

    if(vendor == 0xFFFF) return;
    //device exists
    // printf("[%X:%X:%X] : %X/%X/%X\n", bus, device, function,
    //  get_base_class(bus, device, function), get_sub_class(bus, device,function),
    //  get_prog_lf(bus, device, function));
    // check_function(bus, device, function);
    pci_add_device(bus, device, function);

    unsigned short header_type = get_header_type(bus, device, function);
    if((header_type & 0x80) != 0){
        // multiple functions so check the remaining
        for(function = 1; function < 8; function++){
            if(get_vendor_id(bus, device, function) != 0xFFFF){
    //             printf("[%X:%X:%X] : %X/%X/%X\n", bus, device, function,
    //  get_base_class(bus, device, function), get_sub_class(bus, device,function),
    //  get_prog_lf(bus, device, function));
                pci_add_device(bus, device, function);
                check_function(bus, device, function);
            }
        }
    }
}

unsigned short get_vendor_id(unsigned char bus, unsigned char slot, unsigned char func ){
    return pci_config_read_word(bus, slot, func, 0);
}

unsigned short get_base_class(unsigned char bus, unsigned char slot, unsigned char func ){
    return (pci_config_read_word(bus, slot, func, 10) >> 8);
}

unsigned short get_prog_lf(unsigned char bus, unsigned char slot, unsigned char func ){
    return (pci_config_read_word(bus, slot, func, 8) >> 8);
}


unsigned short get_sub_class(unsigned char bus, unsigned char slot, unsigned char func ){
    return (pci_config_read_word(bus, slot, func, 10) & 0xFF);
}

unsigned short get_secondary_bus(unsigned char bus, unsigned char slot, unsigned char func ){
    return (pci_config_read_word(bus, slot, func, 24) >> 8);
}


unsigned short get_header_type(unsigned char bus, unsigned char slot, unsigned char func){
    unsigned short header_type = pci_config_read_word(bus, slot, func, 14);
    return header_type & 0xFF;
}

void check_bus(unsigned char bus){
    unsigned char device;
    //printf("CHECKING BUS %X\n", bus );
    for(device = 0; device < 32; device++){
        check_device(bus, device);
    }
}

void check_all_buses(void){
    unsigned short function;
    unsigned short bus;

    unsigned short header_type = get_header_type(0, 0, 0);

    if((header_type & 0x80) == 0){
        // single PCI host
        check_bus(0);
    }else{
        for(function = 0; function < 8; function++){

            if (get_vendor_id(0, 0, function) != 0xFFFF) break;
            bus = function;
            check_bus(bus);
        }
    }
}