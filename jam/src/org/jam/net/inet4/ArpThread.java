package org.jam.net.inet4;

/**
 * 
 * @author Joe Kulig
 * created April 9, 2018
 * Copyright 2018, All Rights Reserved
 *
 * The class will run as a thread and handle ARP requests
 */
public class ArpThread 
implements Runnable
{
    private static ArpTable arpTable;
    
    public void run()
    {
        while(true)
        {
            
        }
    }

    public void reply(Arp arpPacket)
    {
        
    }
    
    static void request(Arp arpPacket)
    {
        
    }
}