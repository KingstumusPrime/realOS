char * strstr ( const char * str, const char * sub){
    int ref = 0; // reference to our part in sub

    while (*str != '\0')
    {
        if(*str == sub[ref]){
            ref++;
            if(sub[ref] =='\0'){
                return str - ref + 1; // plus one to ignore the '\0'
            }
        }else{
            ref = 0;
        }

        str++;
    }
    
}