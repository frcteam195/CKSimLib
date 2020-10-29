package com.team195;

import edu.wpi.first.wpilibj.Sendable;
import edu.wpi.first.wpilibj.smartdashboard.SendableBuilder;
import edu.wpi.first.wpilibj.smartdashboard.SendableRegistry;

public class CKSimMotor implements Sendable {
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
        SendableRegistry.addLW(this, "CKSimMotor", id);
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

    @Override
    public void initSendable(SendableBuilder builder) {
        builder.setSmartDashboardType("CKSimMotor");
        // builder.setActuator(false);
        // builder.setSafeState(this::setDisabled);
        builder.addDoubleProperty("Value", this::getMotorValue, value -> setMotorValue((float) value));
    }
}
