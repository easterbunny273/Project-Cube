function start2()
  print "lua-method \"start2\": we will try to call a c++ method"

  -- call some c++ methods
  TestMethodCalledFromLua(99);
  TestMethodCalledFromLua(100);

  -- get an instance of a class from a c++-method
  instance1 = TestMethodReturnClass();

  -- call a c++ method with that instance
  TestMethodClass(instance1);
end
