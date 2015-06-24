   import javax.swing.JFrame;

/**
 * DotDriver class adds the panel in to the JFrame
 * @author Jesse
 */

   public class DotDriver {
   /**
    * Creates a frame and adds the dot panel
    */
      public static void main(String[] args){
         JFrame gui=new JFrame("Drawing Dot");
         DotPanel dotPanel=new DotPanel();
         gui.setSize(800,650);
         gui.add(dotPanel);
         gui.setVisible(true);	
         gui.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);	
      }
   }
