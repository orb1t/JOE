/**
 * Created on Sep 25, 2016
 *
 * Copyright (C) Joe Kulig, 2016
 * All rights reserved.
 */
package org.jikesrvm.scheduler;
import test.org.jikesrvm.org.gctests.Exhaust;
import test.org.jikesrvm.basic.core.bytecode.*;
import test.org.jikesrvm.basic.core.threads.TestDispatch;

/**
 * @author Joe Kulig
 *
 */
public class TestThread extends Thread {
    public void run()
    {
//        System.out.println("Exhaust test");
//        // Exhaust.main(null);
//        TestArithmetic.main(null);
//        TestArrayAccess.main(null);
//        TestClassHierarchy.main(null);
//        try
//        {
//          TestClone.main(null);
//        }
//        catch (CloneNotSupportedException e)
//        {
//          // TODO Auto-generated catch block
//          e.printStackTrace();
//        }
//        TestCompare.main(null);
//        TestConstants.main(null);
//        TestFieldAccess.main(null);
//        TestFinally.main(null);
//        TestInstanceOf.main(null);
//        TestInvoke.main(null);
//        TestMiranda.main(null);
//        TestResolveOnCheckcast.main(null);
//        TestResolveOnInstanceof.main(null);
//        TestResolveOnInvokeInterface.main(null);
//        TestReturn.main(null);
//        TestSwitch.main(null);
//        TestFloatingRem.main(null);
//        TestThrownException.main(null);
        TestDispatch.main(null);
    }
}
