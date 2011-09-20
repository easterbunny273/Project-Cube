function start2()
  print "lua-method \"start2\": we will try to call a c++ method"

  -- call some c++ methods
  TestMethodCalledFromLua(99);
  TestMethodCalledFromLua(100);

  -- get an instance of a class from a c++-method
  instance1 = TestMethodReturnClass(0);

  -- call a c++ method with that instance
  TestMethodClass(instance1);

  instance2 = TestClass(100)
  TestMethodClass(instance2)

  event_instance = CreateEventA();
  event_instance.m_iTest1 = 99;
  TestMethodEvent(event_instance);

  event_instance2 = CreateEventB();
  event_instance2.m_iTest = 99;
  TestMethodEvent(event_instance2);
end
