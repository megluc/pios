

extern char __bss_start[];
extern char __bss_end[];

void clear__bss(){
	char *bss = __bss_start;
	while(bss < __bss_end) {
		*bss++ = 0;
   }
}


void kernel_main() {
// adding a comment
// another comment

	clear__bss();

	
      
      char *bssstart, *bssend;
 

      bssstart = __bss_start;
      bssend = __bss_end;
      
    while(1){
    }
}
