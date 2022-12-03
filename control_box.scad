

module topMountingScrews() {
    difference() {
        cylinder(d=8, h=40);
        translate([0,0,35]) {
           cylinder(d=4, h=15);
        };
        cylinder(d=8, h=20);
    }
}

module lc_usb_mounting_screws() {
    difference(){
        cylinder(d=8, h=8);
        cylinder(d=4, h=8);
    }
    translate([0,18,0]){
        difference(){
            cylinder(d=8, h=8);
            cylinder(d=4, h=8);
        }   
    }
    translate([39,0,0]){
        difference(){
            cylinder(d=8, h=8);
            cylinder(d=4, h=8);
        }   
    }
    translate([39,18,0]){
        difference(){
            cylinder(d=8, h=8);
            cylinder(d=4, h=8);
        }   
    }
}

module lc_amp_mount_screws(){
    difference(){
        cylinder(d=8, h=8);
        cylinder(d=4, h=8);
    }
    translate([15,19,0]){
        difference(){
            cylinder(d=8, h=8);
            cylinder(d=4, h=8);
        }
    }
}

translate([7.5, 45, 2.5]) {
    lc_usb_mounting_screws();
}

translate([65,45, 2.5]) {
    lc_amp_mount_screws();    
}
translate([30,20, 2.5]) {
    rotate([0,0,90]){
        lc_amp_mount_screws();    
    }
}



module aluminium_mount() {
    difference() {
        hull() {
            cylinder(d=15, h=2);
            translate([0, 40, 0]) {
                cylinder(d=15, h=2);    
            };
        
            translate([20, -10, 0]) {
                cylinder(d=15, h=2);
            };
            translate([20, 50, 0]) {
                cylinder(d=15, h=2);
            };
        }
        cylinder(d=6.5, h=5);
        translate([0, 40, 0]) {
            cylinder(d=6.5, h=5);
        }
    }
}

difference() {
    cube([90, 80, 40]);
    translate([2.5, 2.5, 2.5]) {
        cube([85, 75, 37.5]);    
    };   
    
    // Pedals
    translate([22, 5, 20]) {
        rotate([90, 00, 0]) {
            cylinder(d=16, h=10);
        };
    };
    
    translate([45, 5, 20]) {
        rotate([90, 00, 0]) {
            cylinder(d=16, h=10);
        };
    };
    
    translate([67, 5, 20]) {
        rotate([90, 00, 0]) {
            cylinder(d=16, h=10);
        };
    };
    
    
    
    // Text
    translate([12, 1, 30]) {
        rotate([90,0,0]) {
            linear_extrude(height=3, convexity=10, twist=0) {
                text("Throttle", size=4);
            };
        };
    }

    translate([38, 1, 30]) {
        rotate([90,0,0]) {
            linear_extrude(height=3, convexity=10, twist=0) {
                text("Brake", size=4);
            };
        };
    }
    translate([59, 1, 30]) {
        rotate([90,0,0]) {
            linear_extrude(height=3, convexity=10, twist=0) {
                text("Clutch", size=4);
            };
        };
    }
    translate([0,47,12]){
        cube([14, 14, 14]);
    }

}

translate([5,5,0]) {
    topMountingScrews();
}
translate([5,75,0]) {
    topMountingScrews();
}
translate([85,5,0]) {
    topMountingScrews();
}
translate([85,75,0]) {
    topMountingScrews();
}

translate([-10, 20, 0]) {
    aluminium_mount();  
};

translate([100, 60, 0]) {
    rotate([0,0,180]) {
        aluminium_mount();  
    }
};