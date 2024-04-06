import java.util.concurrent.Executor;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class SumThread {
     static Integer sum = Integer.valueOf(0);

    public static void main(String[] args) {
        ExecutorService executor = Executors.newCachedThreadPool();
        int c = 1;
        while(c <= 1000) {
            executor.execute(new SumThread1()); // execute(): doesn't return value while submit() return Future type that we can handle exception
            c++;
        }
        executor.shutdown();
        while(!executor.isTerminated()) {
            Thread.yield();

        }
        System.out.println("The sum is " + sum);
        System.out.println("-----End of Main-----");
    }

    static public synchronized void SUM() {
        sum += 1;
    }

    static class SumThread1 implements Runnable {
        @Override
        public void run() {
            // add 1 to the variable sum
            SUM();
        }
    }

}
