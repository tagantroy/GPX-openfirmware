difference() {
    cube([80, 80, 2]);
    translate([5,5,0]) {
        cylinder(d=4, h=5);
    }
    translate([5,75,0]) {
        cylinder(d=4, h=5);
    }
    translate([75,5,0]) {
        cylinder(d=4, h=5);
    }
    translate([75,75,0]) {
        cylinder(d=4, h=5);
    }
};
