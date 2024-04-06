import java.util.LinkedList;
import java.util.Random;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class ProducerConsumer {
    private static SharedBuffer buffer = new SharedBuffer();
    private static Lock lock = new ReentrantLock();
    private static Condition adding = lock.newCondition();
    private static Condition removing = lock.newCondition();

    public static void main(String[] args) {
        ExecutorService executor = Executors.newCachedThreadPool();
        int c=1;
        while(c <= 20) {
            executor.execute(new Producer());
            executor.execute(new Consumer());
            c++;
        }
        executor.shutdown();
        while(!executor.isTerminated()) {
            Thread.yield();
        }
        System.out.println("-----End of Main-----");

    }
    static class Producer extends Thread {
        @Override
        public void run() {
            // produce integer values
            Random rand = new Random();
            int value = rand.nextInt(0, 100);
            // add them to the shared buffer
            try {
                buffer.write(value);
            } catch (InterruptedException e){}
        }
    }
    static class Consumer extends Thread {
        @Override
        public void run() {
            // consume integer values from the shared buffer
            try {
                buffer.read();
            } catch (InterruptedException e){}
        }
    }
    static class SharedBuffer extends LinkedList {
        // limited size (Consider max size is 10)
        public void write(int value) throws InterruptedException{
            lock.lock();

            // add integer value to the buffer
            if( buffer.size() >= 0 && buffer.size() < 10 ) {
                try {
                    super.addLast(value);
                    System.out.println("Element was added to the buffer");
                    adding.await();
                } finally {
                    lock.unlock();
                }

            }
            else { // buffer size == 10
                try {
                    removing.signal();
                }
                finally {
                    lock.unlock();
                }
            }
        }
        public void read() throws InterruptedException{
            lock.lock();
            if(buffer.size() > 0) {
                try {
                    // read and remove integer value from the buffer
                    String val = super.removeFirst().toString();
                    System.out.println(val + " was removed from the buffer");
                    removing.await();
                } finally {
                    lock.unlock();
                }
            }
            else { // buffer size == 0
                try {
                    adding.signal();
                } finally {
                    lock.unlock();
                }
            }
        }
    }
}
