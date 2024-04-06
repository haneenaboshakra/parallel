import java.util.LinkedList;
import java.util.concurrent.Executor;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;
import java.util.Date;


public class AllProducersAllConsumers {
    private static Lock lock = new ReentrantLock();
    private static Condition adding = lock.newCondition();
    private static Condition removing = lock.newCondition();

public static MyQueue q = new MyQueue();
    public static void main(String[] args) {
        ExecutorService executor = Executors.newCachedThreadPool();
        int c = 1;
        while(c <= 20) {
            executor.execute(new Producer());
            executor.execute(new Consumer());
            c++;
        }
        executor.shutdown();
        while(!executor.isTerminated()) {
            Thread.yield();
            System.out.println("The current size is  " + q.size());
        }
        System.out.println("----End OF MAIN ----");
        System.out.println("The final size is  " + q.size());
    }
    static class Producer extends Thread{
        @Override
        public void run() {
            Date d = new Date();
            try {
                q.addElement(d);
            } catch(InterruptedException e) {}
        }
    }
    static class Consumer extends Thread{
        @Override
        public void run() {
            try {
                q.removeElement();
            } catch(InterruptedException e){}
        }
    }
    static class MyQueue extends LinkedList {
        public void addElement(Date d) throws InterruptedException{
            lock.lock();
            while(q.size() == 10){
                adding.await();
            }
            if(q.size() < 10) {
                try {
                    super.addLast(d);
                    System.out.println(d.toString()+"  is added");
                    removing.signal();
                }
                finally {
                    lock.unlock();
                }
            }

        }
        public void removeElement() throws InterruptedException{
            lock.lock();
            while(q.size() == 0)
                removing.await();
            if (q.size() > 0) {
                try {
                    super.removeFirst();
                    System.out.println("An element will be removed");
                    adding.signal();
                }
                finally {
                    lock.unlock();
                }

            }

        }
    }
}
