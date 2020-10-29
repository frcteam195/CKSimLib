package com.team195;

import edu.wpi.first.wpilibj.Sendable;
import edu.wpi.first.wpilibj.smartdashboard.SendableBuilder;
import edu.wpi.first.wpilibj.smartdashboard.SendableRegistry;

public class CKSimAdvancedObject implements Sendable {
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
        SendableRegistry.addLW(this, "CKSimAdvanced", id);
    }

    public float getValue() {
        return CKSim.getInstance().getAdvObjValue(id);
    }

    private boolean validateId(int id) {
        return (id >= 0 && id < CKSim.MAX_NUM_ADVOBJ);
    }

    @Override
    public void initSendable(SendableBuilder builder) {
        builder.setSmartDashboardType("CKSimAdvanced");
        // builder.setActuator(false);
        // builder.setSafeState(this::setDisabled);
        builder.addDoubleProperty("Value", this::getValue, null);
    }
}
