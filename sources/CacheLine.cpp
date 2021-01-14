#include <CacheLine.h>

CacheLine::CacheLine (bool _wb)
{
    v = false;  //valid bit
    k = false; // the boolean needed in the handle miss
    tag = 0;
    hits = 0;
    misses = 0;
    cache_reads = 0;
    cache_writes = 0;
    mem_reads = 0;
    mem_writes = 0;
    wb = _wb;
}
void CacheLine::handle_miss (uint64_t p_tag)
{
    //Write your code here
        v = true;       //set the validity but to 1
        tag = p_tag;    //set the tag to p_tag
        misses++;       //increment the misses

    if(wb == false){
        if(mode1 == READ_ACCESS){      //check if the mode is R
            cache_reads++;      //increment the cache reads
            mem_reads++;        //increment the mem_reads
        }else if(mode1 == WRITE_ACCESS){      //check if the mode is W
            mem_writes++;       //increment the mem_writes
            cache_writes++;     //increment the cache_writes
            mem_reads++;        //increment the mem_reads
        }
    }else if(wb = true){
        if(mode1 == READ_ACCESS){      //check if the mode is R
            if(k == true){
                k = false;          //reset the flag
                cache_reads++;      //increment the cache reads
                mem_reads++;        //increment the mem_reads
                mem_writes++;       //increment the mem_writes
            }else if (k == false){  //check if the k is false
                cache_reads++;      //increment the cache_reads
                mem_reads++;        //increment the mem_reads
            }
        }else if(mode1 == WRITE_ACCESS){      //check if the mode is W
            if(k ==true){
                cache_writes++;     //increment the cache_writes
                mem_reads++;        //increment the mem_reads
                mem_writes++;       //increment the mem_writes
            }else if(k == false){
                k = true;           //set k with true
                cache_writes++;     //increment the cache_write
                mem_reads++;        //increment the mem_reads
            }
            
        }
    }
}

bool CacheLine::valid()
{
    return v;
}
uint64_t CacheLine::getTag()
{
    return tag;
}
uint64_t CacheLine::getHits()
{
    return hits;
}
uint64_t CacheLine::getMisses()
{
    return misses;
}
uint64_t CacheLine::getCacheReads()
{
    return cache_reads;
}
uint64_t CacheLine::getCacheWrites()
{
    return cache_writes;
}
uint64_t CacheLine::getMEMReads()
{
    return mem_reads;
}
uint64_t CacheLine::getMEMWrites()
{
    return mem_writes;
}
void CacheLine::access (uint64_t p_tag, char mode)
{
   //Write your code here
   mode1 = mode;   //mode from the .h file to be able to access in the handle miss

   if(p_tag == tag){    //check if the tag is equal to p_tag
        if(v == true){
            hits++;     //if so increase the hits
            if(mode1 == READ_ACCESS)     //checkk the mode to be R or W
                cache_reads++;      //increment the cache reads when R is selected
            else{
                k = true;       //set the bool value from the .h to true
                cache_writes++;     //increment the cache writes
                if(wb == false){     //check if the wb is false
                    mem_writes++;       //increment the mem writes if the wb is false
                }       
            }
        }
   }else{
       handle_miss(p_tag);     //otherwise call handle miss 
    }          

}

CacheLine::~CacheLine()
{

}