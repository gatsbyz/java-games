package com.gatsby;


import java.awt.Point;
   import java.awt.Color;

/**
 * DotLinkedList is a class for the linked list of Dots
 * @author Jesse
 * 
 */
   public class DotLinkedList {
      private DotNode head;
      private int size;
   
   /**
    * Adds the Dot to the end of the list
    * 
    * @param dot
    */
      public void add(Dot dot){
         DotNode posNode=head;
         if (posNode!=null){
            while(posNode.next!=null){
               posNode=posNode.next;	
            }
            posNode.next=new DotNode(dot);
         }
         else head=new DotNode(dot);
         size++;
      
      }
   
   /**
    * Removes the Dot from the list
    * @param dot
    */
      public void remove(Dot dot){
         DotNode posNode=head;
         if (posNode!=null){
            if (posNode.dot.equals(dot)){
               head=head.next;
               size--;}
            else{
               while(posNode.next!=null){
                  if (posNode.next.dot.equals(dot)){
                     posNode.next=posNode.next.next;
                     size--;
                     break;}
                  posNode=posNode.next;
               }
            }
         }
      
      }
   
   /**
    * Gets the Dot at the specified index. If the index
    * does not occur within the list, return null
    * @param i index
    * @return the dot
    */
      public Dot get(int i){
         DotNode posNode=head;
         for(int j=0;j<i;j++){
            if (posNode==null)
               return null;
            posNode=posNode.next;}
         return posNode.dot;
      }
   
   /**
    * Get the dot that contains the specified Point. If
    * such a dot does not exist within the list, return null.
    * 
    * @param p
    * @return
    */
      public Dot getDotAtPoint(Point p){
         DotNode posNode=head;
         while(posNode!=null){
            if (posNode.dot.contains(p))
               return posNode.dot;
            posNode=posNode.next;
         }
         return null;
      }
   
   /**
    * Remove all of the dots within the list that 
    * have the specified color
    * 
    * @param c
    */
      public void removeAllByColor(Color c){
         DotNode posNode=head;
         if(posNode!=null){
            if (posNode.dot.getColor().equals(c)){
               head=head.next;
               size--;
               removeAllByColor(c);}
            else{
               while((posNode!=null)&&(posNode.next!=null)){
                  if (posNode.next.dot.getColor().equals(c)){
                     posNode.next=posNode.next.next;
                     size--;}
                  else posNode=posNode.next;}}}}
   
   /**
    * Get the size of the list
    * @return
    */
      public int size(){
         return size;}
   
   /**
    * Get the count of dots that are the specified
    * color within the list.
    *
    * @return 
    */
      public int getColorCount(Color c){
         DotNode posNode=head;
         int count=0;
         while (posNode!=null){
            if(posNode.dot.getColor().equals(c))
               count++;
            posNode=posNode.next;}
         return count;}
   
   /**
    * Get a string representation of the list
    */
      public String toString(){
         String dotList="";
         DotNode posNode=head;
         if(posNode!=null){
            dotList+=posNode.dot.toString();}
         return dotList;}
   
   /**
    * Class to contain a dot and the reference to the 
    * next DotNode in the list
    * 
    */
      private class DotNode{
         private Dot dot;
         private DotNode next;
      /**
       * Create a new DotNode to contain the specified Dot
       * 
       * @param dot
       */
         public DotNode(Dot dot){
            this.dot=dot;}
      
      /**
       * String representation of the node. This should
       * just be the String representation of the Dot that
       * the node contains
       */
         public String toString(){
            return dot.toString();}
      }
   }
