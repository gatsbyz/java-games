   import java.awt.Color;
   import java.awt.Graphics;
   import java.awt.Point;
   import javax.swing.JPanel;
   import java.awt.event.KeyEvent;
   import java.awt.event.KeyListener;
   import java.awt.event.MouseListener;
   import java.awt.event.MouseEvent;
   
/**
 * DotPanel class creates a panel for creating the dots
 * @author Jesse
 */
   public class DotPanel extends JPanel {
      private DotLinkedList dotLinked;
      private boolean paintString=true;
      private final int HEIGHT=600;
      private Color color = Color.red;   
      
      public DotPanel(){
         addMouseListener(new DotListener());
         addKeyListener(new CanvasListener());
         dotLinked=new DotLinkedList();
         setFocusable(true);
      }
   /**
    * puts color into the chosen dot
    * 
    * @param g 
    */
      public void paint(Graphics g){
         super.paint(g);
         if (dotLinked.size()>0){
            for(int i=0; i<dotLinked.size();i++){
               dotLinked.get(i).draw(g);
               if(paintString){
                  g.setColor(Color.black);
                  g.drawString(dotLinked.get(i).toString(), 10, 15*i+10);
               }
            }
         }
      
         g.setColor(Color.black);
         g.drawString("There are "+dotLinked.getColorCount(Color.red)+ " red dots, "+
            dotLinked.getColorCount(Color.blue)+" blue dots, " +
            dotLinked.getColorCount(Color.green)+" green dots" ,20,HEIGHT-30);}
   
   /**
    * DotListener is a private class implementing MouseListener 
    */
      private class DotListener implements MouseListener{
      
         Point pClick;
      /**
       * @param e 
       */
         public void mousePressed(MouseEvent e) {	
            pClick=e.getPoint();
            Dot dotsClick=dotLinked.getDotAtPoint(pClick);
            if(dotsClick==null){
               dotLinked.add(new Dot(pClick, color));}
            else{
               dotLinked.remove(dotsClick);}
            repaint();
         }
      
         public void mouseDragged(MouseEvent e) {}
      
         public void mouseReleased(MouseEvent e) {}
      
         public void mouseMoved(MouseEvent e) {}
      
         public void mouseClicked(MouseEvent e) {}
      
         public void mouseEntered(MouseEvent e) {}
      
         public void mouseExited(MouseEvent e) {}
      }
   /**
    * CanvasListener class implements KeyListener    
    */
      private class CanvasListener implements KeyListener {
      /**
       * @param e 
       */
         public void keyPressed(KeyEvent e) {
            switch (e.getKeyCode()) {
               case KeyEvent.VK_R:
                  color=Color.red;
                  break;
               case KeyEvent.VK_G:
                  color=Color.green;
                  break;
               case KeyEvent.VK_B:
                  color=Color.blue;
                  break;
               case KeyEvent.VK_1 :
                  dotLinked.removeAllByColor(Color.red);
                  break;
               case KeyEvent.VK_2 :
                  dotLinked.removeAllByColor(Color.green);
                  break;
               case KeyEvent.VK_3 :
                  dotLinked.removeAllByColor(Color.blue);
                  break;
               case KeyEvent.VK_S :
                  if(paintString){
                     paintString=false;}
                  else
                     paintString=true;
                  break;}
            repaint();
         }
         public void keyReleased(KeyEvent e) {
         }
         public void keyTyped(KeyEvent e) {
         }
      
      }
   
   }
