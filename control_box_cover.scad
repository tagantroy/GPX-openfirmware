difference() {
    cube([90, 80, 2]);
    translate([5,5,0]) {
        cylinder(d=4, h=5);
    }
    translate([5,75,0]) {
        cylinder(d=4, h=5);
    }
    translate([85,5,0]) {
        cylinder(d=4, h=5);
    }
    translate([85,75,0]) {
        cylinder(d=4, h=5);
    }
};
