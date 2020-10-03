package com.team195;

public class CKSimAdvancedObject {
    private int id;

    public CKSimAdvancedObject(int id) {
        if (!validateId(id)) {
            System.out.println("Invalid advanced object ID: " + id);
            return;
        }
        if (!CKSim.getInstance().registerAdvObj(id)) {
            System.out.println("Failed to add advanced object ID: " + id + "; ID Already in use!");
            return;
        }
        this.id = id;
    }

    public float getValue() {
        return CKSim.getInstance().getAdvObjValue(id);
    }

    private boolean validateId(int id) {
        return (id >= 0 && id < CKSim.MAX_NUM_ADVOBJ);
    }
}
