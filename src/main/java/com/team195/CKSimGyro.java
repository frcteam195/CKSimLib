package com.team195;

public class CKSimGyro {
    private int id;
    private int numAxes;

    public CKSimGyro(int id, int numAxes) {
        if (!validateId(id)) {
            System.out.println("Invalid gyro ID: " + id);
            return;
        }
        if (!validateId(id + numAxes - 1)) {
            System.out.println("Invalid last axis gyro ID: " + (id + numAxes - 1));
            return;
        }
        for (int i = id; i < id + numAxes; i++) {
            if (!CKSim.getInstance().registerGyro(i)) {
                System.out.println("Failed to add gyro ID: " + i
                        + "; ID Already in use! Please make sure ids for additional axes are not in use. Each axis consumes the next consecutive id.");
                return;
            }
        }
        this.id = id;
        this.numAxes = numAxes;
    }

    public float getValue(int axis) {
        if (axis >= 0 && axis < numAxes && validateId(id + axis) && CKSim.getInstance().containsGyro(id + axis)) {
            return CKSim.getInstance().getGyroValue(id + axis);
        } else {
            System.out.println("Invalid axis: " + axis);
        }
        return 0;
    }

    private boolean validateId(int id) {
        return (id >= 0 && id < CKSim.MAX_NUM_GYRO);
    }

}
