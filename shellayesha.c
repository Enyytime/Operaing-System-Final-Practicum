void mv(byte cwd, char* src, char* dst){
    struct node_fs node_fs_buf;
    int i;
    int len;
    int src_index = -1;
    int dst_index = -1;
    byte parent_index;
    char filename[64];

    readSector(&(node_fs_buf.nodes[0]), FS_NODE_SECTOR_NUMBER);
    readSector(&(node_fs_buf.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);

    for(i=0; i<FS_MAX_NODE; i++){
        if(strcmp(node_fs_buf.nodes[i].node_name, src) && node_fs_buf.nodes[i].parent_index == cwd){
            src_index = i;
            break;
        }
    }

    if(src_index == -1){
        printString("Error: Source not found\n");
        return;
    }

    if(node_fs_buf.nodes[src_index].data_index == FS_NODE_D_DIR){
        printString("Error: Source is a directory\n");
        return;
    }

    if(dst[0] == '/'){
        parent_index = FS_NODE_P_ROOT;
        strcpy(filename, dst + 1);
    }else if(dst[0] == '.' && dst[1] == '.' && dst[2] == '/'){ 
        parent_index = node_fs_buf.nodes[cwd].parent_index;
        strcpy(filename, dst + 3);
    }else{
        for(i = strlen(dst) - 1; i >= 0; i--){
            if(dst[i] == '/'){
                dst[i] = '\0';
                break;
            }
        }

        if(i>=0){
            parent_index = cwd;
            for(i=0; i<FS_MAX_NODE; i++){
                if(strcmp(node_fs_buf.nodes[i].node_name, dst) && node_fs_buf.nodes[i].parent_index == cwd){
                    if(node_fs_buf.nodes[i].data_index == FS_NODE_D_DIR){
                        parent_index = i;
                        break;
                    }else{
                        printString("Error: Destination is not a directory\n");
                        return;
                    }
                }
            }

          strcpy(filename, dst + len + 1);
        }else{
            parent_index = cwd;
            strcpy(filename, dst);
        }
    
    }
  
    for(i=0; i<FS_MAX_NODE; i++){
        if(strcmp(node_fs_buf.nodes[i].node_name, filename) && node_fs_buf.nodes[i].parent_index == parent_index){
            printString("Error: File already exists\n");
            return;
        }
    }

    strcpy(node_fs_buf.nodes[src_index].node_name, filename);
    node_fs_buf.nodes[src_index].parent_index = parent_index;

    writeSector(&(node_fs_buf.nodes[0]), FS_NODE_SECTOR_NUMBER);
    writeSector(&(node_fs_buf.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);


    printString("File moved\n");
}