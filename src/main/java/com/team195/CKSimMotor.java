package com.team195;

public class CKSimMotor {
    private int id;

    public CKSimMotor(int id) {
        if (!validateId(id)) {
            System.out.println("Invalid motor ID: " + id);
            return;
        }
        if (!CKSim.getInstance().registerMotor(id)) {
            System.out.println("Failed to add motor ID: " + id + "; ID Already in use!");
            return;
        }
        this.id = id;
    }

    public float getMotorValue() {
        return CKSim.getInstance().getMotorValue(id);
    }

    public synchronized void setMotorValue(float value) {
        CKSim.getInstance().setMotorValue(id, value);
    }

    public float getEncoderValue() {
        return CKSim.getInstance().getEncoderValue(id);
    }

    private boolean validateId(int id) {
        return (id >= 0 && id < CKSim.MAX_NUM_MOTORS);
    }
}
