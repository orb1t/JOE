package org.jam.tools;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.List;

import gnu.java.lang.CPStringBuilder;

public class CommandProcessor
implements Runnable
{
    private final Lexer lexer;
    private final ObjectReader reader;
    private final RvmMap map;
    private String[] results;
    
    public CommandProcessor(ObjectReader reader)
    {
        lexer = new Lexer();
        this.reader = reader;
        map = reader.getMap();
    }
    
    public void run()
    {
        BufferedReader input = new BufferedReader(new InputStreamReader(System.in));
        while(true)
        {
            System.out.print("## ");
            String command=null;
            try
            {
                command = input.readLine();
                results = lexer.lex(command);
                if(results == null) continue;
                if(results[0].equals("p"))
                {
                    try
                    {
                        int address = parseNumber(results[1]);
                        reader.dumpObject(address);
                    }
                    catch(NumberFormatException nfe)
                    {
                        List<MapConstants> constantList;
                        List<MapField> fieldList = map.getField(results[1]);
                        if(fieldList.isEmpty())
                        {
                            // Try the constant map
                            constantList = map.getLiteral(results[1]);
                            if(constantList.isEmpty())
                            {
                                System.out.println("Field not found!");
                            }
                            else
                            {
                                reader.dumpObject(constantList.get(0));
                            }
                        }
                        else if(fieldList.size()==1)
                        {
                            reader.dumpObject(fieldList.get(0));
                        }
                        else if(fieldList.size()==0)
                        {
                            System.out.println("Field not found!");
                        }
                        else
                        {
                            for(MapField field: fieldList)
                            {
                                System.out.println(field.getKey());
                            }
                        }
                    }
                }
                else if(results[0].equals("c"))
                {
                    try
                    {
                        int address = parseNumber(results[1]);
                        MapCode code = map.getCode(address);
                        System.out.println(code.getTypeName()+"."+code.getName()+code.getParameters());
                    }
                    catch (NumberFormatException nfe)
                    {
                        System.err.println("Bad number: "+results[1]);
                    }
                }
                else if(results[0].equals("d"))
                {
                    int address = 0x100000;
                    int length = 32;
                    
                    if(results.length < 2)
                    {
                        System.err.println("usage: d <address> [length]");
                        continue;
                    }
                    address = parseNumber(results[1]);
                    if(results.length==3)
                    {
                        length = parseNumber(results[2]);
                    }
                    dump(address, length);
                }
                else if(results[0].equals("bt"))
                {
                    int address = parseNumber(results[1]);
                    int stackPointer = reader.readInt(address);
                    int ipAddress;
                    for(; stackPointer > 0;)
                    {
                        ipAddress = reader.readInt(stackPointer+4);
                        MapCode code = map.getCode(ipAddress);
                        System.out.print(formatInt(stackPointer, 8)+": ");
                        if(code == null)
                        {
                            System.out.println(formatInt(ipAddress,8));
                        }
                        else
                        {
                            System.out.println(code.getTypeName()+"."+code.getName()+code.getParameters());
                        }
                        stackPointer = reader.readInt(stackPointer);
                    }
                }
                else
                {
                    help();
                }

            } catch (Exception e)
            {
                e.printStackTrace();
            }
        }
    }

    private void help()
    {
        System.out.println("p <object address|field name>\t\tDisplay contents of object address or field");
        System.out.println("c <address>\t\tDisplay method name of address");
        System.out.println("d <address>\t\tHex dump starting at address");
        System.out.println("bt <address> [length]\t\tBack trace stack address");
    }

    private void dump(int address, int size)
    {
        StringBuilder prefix;
        int index = 0;
        int columns = 0;
        for(; index < size; index+=4)
        {
            prefix = formatInt(address+index*4, 8);
            prefix.append(": ");
            if(size - index < 4) columns = size - index;
            else columns = 4;
            for(int nextColumn=0; nextColumn < columns; nextColumn++)
            {
                int value = reader.readInt(address+nextColumn*4+index*4);
                prefix.append(formatInt(value, 8)).append(' ');
            }
            /*
             * print out any characters
             */
            for (int j = 0; j < columns*4; j++)
            {
                byte aByte = reader.readByte(address+index*4+j);
              if ((aByte & 0xFF) < 0x20 || (aByte & 0xFF) > 0x7E)
                  prefix.append('.');
              else
                  prefix.append((char) (aByte & 0xFF));
            }
            System.out.println(prefix.toString());
        }
        
    }

    /**
     * Format an integer into the specified radix, zero-filled.
     *
     * @param i The integer to format.
     * @param radix The radix to encode to.
     * @param len The target length of the string. The string is
     *   zero-padded to this length, but may be longer.
     * @return The formatted integer.
     */
    public static StringBuilder formatInt(int i, int len)
    {
      String s = Integer.toHexString(i);
      StringBuilder buf = new StringBuilder();
      for (int j = 0; j < len - s.length(); j++)
        buf.append("0");
      buf.append(s);
      return buf;
    }

    /**
     * Return a number from the string
     * @param parameter
     * @return number
     */
    static public int parseNumber(String parameter)
    {
        /*
         * All strings are assumed be in hex.
         * To specify a decimal precede with a 'i' -> i999
         * To specify a hex precede with 'x' or '0x'
         * To specify octal precede with 'o'
         */
        Integer number=null;
        
        char prefix=parameter.charAt(0);
        if(prefix=='i')
        {
            number = Integer.valueOf(parameter.substring(1), 10);
        }
        else if(prefix=='x')
        {
            number = Integer.valueOf(parameter.substring(1), 16);
        }
        else if(prefix=='o')
        {
            number = Integer.valueOf(parameter.substring(1), 8);
        }
        else if(parameter.startsWith("0x"))
        {
            number = Integer.valueOf(parameter.substring(2), 16);
        }
        else
        {
            number = Integer.valueOf(parameter, 16);
        }
        return number;
    }

    public String getArg(int argIndex)
    {
        return results[argIndex];
    }
    
    public String[] getArgs()
    {
        return results;
    }
    public int numberOfArgs()
    {
        return results.length;
    }
    public static void main(String args[])
    {
        CommandProcessor cp = new CommandProcessor(null);
        cp.run();
    }
}
