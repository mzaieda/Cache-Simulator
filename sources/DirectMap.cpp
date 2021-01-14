#include <DirectMap.h>
#include <Utils.h>
#include <iostream>
using namespace std;


DirectMap::DirectMap (uint16_t p_cache_line_size , uint16_t p_cache_size, bool _wb)
{
    //Write your code here
    cache_line_size = p_cache_line_size;    //set the cache_line_size with the coming parameter
    cache_size = p_cache_size;              //set the cache_size with the coming parameter

    offset = log2(cache_line_size);         //calculate the offset
    index = log2(cache_size);               //calculate the index
    tag = 64 - (offset + index);            //calculate the tag

    //loop to create a new cache line for as much as the cache size
    for(int i = 0; i < cache_size; i++){    
        cache[i] = new CacheLine(_wb);
    }
}
void DirectMap::access (uint64_t full_address,char mode)
{
    //Write your code here
    int b_offsets;          //bit wise offset
    int b_index;            //bit wise index
    int b_tag;              //bit wise tag

    Utils ut;           //instantiate an object of utils to get access ot extract bit
    ls_bits = ut.extract_bits(full_address, cache_line_size, 0);  //to complete the .h file
    cs_bits = ut.extract_bits(full_address, cache_size, 0);       //to complete the .h file
    b_offsets = ut.extract_bits(full_address, offset, 0);          //extract bit for the offset which is the first part of the cache line
    b_index = ut.extract_bits(full_address, index, offset);         //extract bit for the index which is after the offset with its size 
    b_tag = ut.extract_bits(full_address, tag, (offset + index));     //extract bit for the tag which is after the offset added to the index
    
    cache[b_index % cache_size]->access(b_tag, mode);         //calling the access with the tag and mode to accesss the cacheline attributes
}
void DirectMap::printStats (unsigned int crc,unsigned int cwc ,unsigned int mrc ,unsigned int mwc)
{
   //Write your code here
   //initializing integer variables by zero for printing
    long t_miss = 0;                 //total miss = 0
    long t_hits = 0;                 //total hits = 0
    long t_cache_reads = 0;          //total cache reads = 0
    long t_cache_writes = 0;         //total cache writes = 0
    long t_memory_reads = 0;         //total memory reads = 0
    long t_memory_writes = 0;        //total memory writes = 0
    long t_access_attempts = 0;       //total memory access = 0
    long cycle_cache_read = 0;       //cycle cache read = 0
    long cycle_cache_writes = 0;     //cycle cache write = 0
    long cycle_cache_access = 0;     //cycle cache access = 0
    long cycle_memory_read = 0;      //cycle memory read = 0
    long cycle_memory_writes = 0;    //cycle memory writes = 0
    long cycle_memory_access = 0;    //cycle memory access = 0

   for (int i=0; i<cache_size; i++){
        t_miss = t_miss + cache[i]->getMisses();        //calculate the total miss
        t_hits = t_hits + cache[i]->getHits();          //calculate the total hits
        t_cache_reads = t_cache_reads + cache[i]->getCacheReads();      //calculate the cache reads
        t_cache_writes = t_cache_writes + cache[i]->getCacheWrites();     //calculate the cache writes
        t_access_attempts = t_access_attempts + (cache[i]->getCacheWrites() + cache[i]->getCacheReads());    ////calculate the access memory attempts
        t_memory_reads = t_memory_reads + cache[i]->getMEMReads();      //calculate the memory reads attemts
        t_memory_writes = t_memory_writes + cache[i]->getMEMWrites();       //calculate the memory writes attempts
        cycle_cache_read = cycle_cache_read + (crc*cache[i]->getCacheReads());      //calculate the cache reads cycle  
        cycle_cache_writes = cycle_cache_writes + (cwc*cache[i]->getCacheWrites());         //calculate the cache writes cycles 
        cycle_cache_access = cycle_cache_access + (crc*cache[i]->getCacheReads() + cwc*cache[i]->getCacheWrites());     //calculate the cache access cycles
        cycle_memory_read = cycle_memory_read + (mrc*cache[i]->getMEMReads());      //calclate the memory reads cycles
        cycle_memory_writes = cycle_memory_writes + (mwc*cache[i]->getMEMWrites());     //calculate the memory writes cycles 
        cycle_memory_access = cycle_memory_access + (mrc*cache[i]->getMEMReads() + mwc*cache[i]->getMEMWrites());       //calculate the memory access cycles
    }

    //printing the values calculated in the previous for loop
    //these are the values listed in the document
    //these values differ from a program to another and thats why we are testing the code on several codes
    cout << "Total Misses: " << t_miss << endl;
    cout << "Total Hits: " << t_hits << endl;
    cout << "Memory Read Access Attempts: " << t_cache_reads << endl;
    cout << "Memory Write Access Attempts: " << t_cache_writes << endl;
    cout << "Total Memory Access Attempts: " << t_access_attempts << endl;
    cout << "Memory Read Access: " << t_memory_reads << endl;
    cout << "Memory Write Access: " << t_memory_writes << endl;
    cout << "Total # of cycles for cache read: " << cycle_cache_read << endl;
    cout << "Total # of cycles for cache write: " << cycle_cache_writes << endl;
    cout << "Total # of cycles for cache access: " << cycle_cache_access << endl;
    cout << "Total # of cycles for memory read: " << cycle_memory_read << endl;
    cout << "Total # of cycles for memory write: " << cycle_memory_writes << endl;
    cout << "Total # of cycles for memory access: " << cycle_memory_access << endl;
}

DirectMap::~DirectMap()
{
    for ( uint16_t i = 0 ; i < cache_size ; i++)
        delete (cache[i]);
}
