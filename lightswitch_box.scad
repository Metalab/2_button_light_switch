wall = 2;

difference()
{

    cube([40,40,38]);
    translate([wall,wall,wall])   
    cube([40-wall*2,40-wall * 2, 38 -wall]);
    
    translate([40* 0.3333,40/2,0])
    cylinder(d=7,wall);
    
    translate([40* 0.6666,40/2,0])
    cylinder(d=7,wall);
    
    translate([40/2-5,0,30])
    cube ([10,10,10]);
    
}

translate([45,0,0])
cube([40,40,2]);