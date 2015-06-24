   import java.awt.Color;
   import java.awt.Graphics;
   import java.awt.Point;

/**
 * The Dot class creates the Dots and customizes it
 * @author Jesse
 */

   public class Dot {
      private static final int DIAMETER = 20;
      private Point point;
      private Color color;
   
   /**
    * Create a new dot at the specified point with the 
    * specified color - note, the point should be the CENTER 
    * of the dot
    * 
    * @param point
    * @param color
    */
      public Dot(Point point, Color color){
         this.point=point;
         this.color=color;}
   
   /**
    * Draw the dot on the graphics object - remember, the point
    * is the dot's center
    * 
    * @param g
    */
      public void draw(Graphics g){
         g.setColor(color);
         g.fillOval(point.x,point.y,DIAMETER,DIAMETER);}
   
   /**
    * Determine if the given point is within the dot
    * 
    * @param p
    * @return
    */
      public boolean contains(Point p){
         if (((p.x-point.x)*(p.x-point.x)+(p.y-point.y)*(p.y-point.y))<=DIAMETER*DIAMETER)
            return true;
         return false;}
   
   /**
    * Determine if the dot is equal to the object. To be equal,
    * the object must be a dot, and it must be at the same location 
    * and have the same color
    * 
    * @return boolean
    */
      public boolean equals(Object o){
         if (o instanceof Dot){
            if((((Dot)o).getColor().equals(color))&&(((Dot)o).getPoint().equals(point)))
               return true;}
         return false;}
   
   /**
    * Get the color of the dot
    * 
    * @return
    */
      public Color getColor(){
         return new Color(color.getRGB());}
   
   /**
    * Get the location of the dot
    * 
    * @return
    */
      public Point getPoint(){
         return new Point(point.x, point.y);}
   
   /**
    * Get a string representation of the dot - this should contain
    * the color of the dot and it's location. If the color is Red, Green, or Blue,
    * it should state that. Otherwise, it should just have the values of rgb for the
    * unknown colors (Hint: Color has a useful toString for getting the rgb values)
    * 
    * @return String
    */
      public String toString(){
         String color;
         if(getColor().equals(Color.red))
            color="Red";
         else if(getColor().equals(Color.blue))
            color="Blue";
         else if(getColor().equals(Color.green))
            color="Green";
         else{ color=getColor().toString();}
         return color+" Dot at " + getPoint().x + " , " + getPoint().y;}
   }
